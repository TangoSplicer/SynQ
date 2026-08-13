(require :asdf)
(asdf:load-system :cffi)

(defun fail (message)
  (format *error-output* "FAIL: ~A~%" message)
  (uiop:quit 1))

(defun require-true (condition message)
  (unless condition (fail message)))

(defparameter *library-path* (car (last sb-ext:*posix-argv*)))
(cffi:load-foreign-library *library-path*)

(cffi:defcfun ("synq_abi_version" synq-abi-version) :unsigned-int)
(cffi:defcfun ("synq_version" synq-version) :pointer)
(cffi:defcfun ("synq_parse_source" synq-parse-source) :int
  (source :string) (out-program :pointer) (out-diagnostic :pointer))
(cffi:defcfun ("synq_export_openqasm3" synq-export-openqasm3) :int
  (program :pointer) (out-openqasm3 :pointer) (out-diagnostic :pointer))
(cffi:defcfun ("synq_string_free" synq-string-free) :void (value :pointer))
(cffi:defcfun ("synq_program_free" synq-program-free) :void (program :pointer))

(defun pointer-string-and-free (pointer)
  (if (cffi:null-pointer-p pointer)
      ""
      (unwind-protect
           (cffi:foreign-string-to-lisp pointer)
        (synq-string-free pointer))))

(handler-case
    (progn
      (require-true (= (synq-abi-version) 1) "Common Lisp consumer observes ABI version 1")
      (require-true (string= (cffi:foreign-string-to-lisp (synq-version)) "synq-c-abi/1")
                    "Common Lisp consumer observes ABI identifier")

      (cffi:with-foreign-objects ((program :pointer) (diagnostic :pointer) (openqasm :pointer))
        (setf (cffi:mem-ref program :pointer) (cffi:null-pointer)
              (cffi:mem-ref diagnostic :pointer) (cffi:null-pointer))
        (let ((status (synq-parse-source "quantum h q[0]
measure q[0]
" program diagnostic)))
          (require-true (= status 0) "Common Lisp consumer parses in-memory source")
          (require-true (not (cffi:null-pointer-p (cffi:mem-ref program :pointer)))
                        "Common Lisp consumer receives an opaque program handle")
          (require-true (cffi:null-pointer-p (cffi:mem-ref diagnostic :pointer))
                        "successful Common Lisp parse has no diagnostic"))

        (setf (cffi:mem-ref openqasm :pointer) (cffi:null-pointer)
              (cffi:mem-ref diagnostic :pointer) (cffi:null-pointer))
        (let ((status (synq-export-openqasm3 (cffi:mem-ref program :pointer) openqasm diagnostic)))
          (require-true (= status 0) "Common Lisp consumer exports parsed source")
          (let ((text (pointer-string-and-free (cffi:mem-ref openqasm :pointer))))
            (require-true (and (search "bit[1] c;" text) (search "c[0] = measure q[0];" text))
                          "Common Lisp consumer receives measurement OpenQASM output"))
          (require-true (cffi:null-pointer-p (cffi:mem-ref diagnostic :pointer))
                        "successful Common Lisp export has no diagnostic"))
        (synq-program-free (cffi:mem-ref program :pointer))

        (setf (cffi:mem-ref program :pointer) (cffi:null-pointer)
              (cffi:mem-ref diagnostic :pointer) (cffi:null-pointer))
        (let ((status (synq-parse-source "measure q[0], q[1]" program diagnostic)))
          (require-true (= status 2) "Common Lisp consumer receives a parse error for malformed memory source")
          (require-true (cffi:null-pointer-p (cffi:mem-ref program :pointer))
                        "failed Common Lisp parse has no opaque program handle")
          (let ((text (pointer-string-and-free (cffi:mem-ref diagnostic :pointer))))
            (require-true (and (search "<memory>:1" text) (search "SYNQ-P008" text))
                          "Common Lisp consumer receives structured in-memory diagnostic"))))
      (format t "SynQ Common Lisp CFFI ABI smoke test passed~%")
      (uiop:quit 0))
  (error (condition)
    (fail (princ-to-string condition))))
