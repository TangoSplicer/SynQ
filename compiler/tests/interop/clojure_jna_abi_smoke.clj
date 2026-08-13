(import '[com.sun.jna NativeLibrary Pointer]
        '[com.sun.jna.ptr PointerByReference])

(defn fail [message]
  (binding [*out* *err*] (println (str "FAIL: " message)))
  (System/exit 1))

(defn require-true [condition message]
  (when-not condition (fail message)))

(defn invoke [function return-type arguments]
  (.invoke function return-type (object-array arguments)))

(defn pointer-string-and-free [free-function pointer]
  (if (or (nil? pointer) (= Pointer/NULL pointer))
    ""
    (let [value (.getString ^Pointer pointer 0)]
      (invoke free-function Void/TYPE [pointer])
      value)))

(try
  (let [library-path (first *command-line-args*)
        library (NativeLibrary/getInstance library-path)
        abi-version (.getFunction library "synq_abi_version")
        version (.getFunction library "synq_version")
        parse-source (.getFunction library "synq_parse_source")
        export-openqasm (.getFunction library "synq_export_openqasm3")
        string-free (.getFunction library "synq_string_free")
        program-free (.getFunction library "synq_program_free")]
    (require-true (= 1 (invoke abi-version Integer/TYPE []))
                  "Clojure consumer observes C ABI version 1")
    (require-true (= "synq-c-abi/1" (.getString ^Pointer (invoke version Pointer [] ) 0))
                  "Clojure consumer observes ABI identifier")

    (let [program (PointerByReference.)
          diagnostic (PointerByReference.)
          parse-status (invoke parse-source Integer/TYPE ["quantum h q[0]\nmeasure q[0]\n" program diagnostic])]
      (require-true (= 0 parse-status) "Clojure consumer parses in-memory source")
      (require-true (not= Pointer/NULL (.getValue program))
                    "Clojure consumer receives opaque program handle")
      (require-true (= Pointer/NULL (.getValue diagnostic))
                    "successful Clojure parse has no diagnostic")

      (let [openqasm (PointerByReference.)
            export-diagnostic (PointerByReference.)
            export-status (invoke export-openqasm Integer/TYPE [(.getValue program) openqasm export-diagnostic])]
        (require-true (= 0 export-status) "Clojure consumer exports parsed source")
        (let [text (pointer-string-and-free string-free (.getValue openqasm))]
          (require-true (and (.contains text "bit[1] c;") (.contains text "c[0] = measure q[0];"))
                        "Clojure consumer receives measurement OpenQASM output"))
        (require-true (= Pointer/NULL (.getValue export-diagnostic))
                      "successful Clojure export has no diagnostic"))
      (invoke program-free Void/TYPE [(.getValue program)]))

    (let [program (PointerByReference.)
          diagnostic (PointerByReference.)
          parse-status (invoke parse-source Integer/TYPE ["measure q[0], q[1]" program diagnostic])]
      (require-true (= 2 parse-status) "Clojure consumer receives parse error for malformed memory source")
      (require-true (= Pointer/NULL (.getValue program))
                    "failed Clojure parse has no opaque program handle")
      (let [text (pointer-string-and-free string-free (.getValue diagnostic))]
        (require-true (and (.contains text "<memory>:1") (.contains text "SYNQ-P008"))
                      "Clojure consumer receives structured in-memory diagnostic"))))
  (println "SynQ Clojure JNA ABI smoke test passed")
  (System/exit 0)
  (catch Throwable error
    (fail (.getMessage error))))
