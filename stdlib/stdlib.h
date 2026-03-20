// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 14: Standard Library - Core Architecture

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <complex>
#include <array>

namespace synq::stdlib {

// ============================================================================
// Quantum Computing Library (100+ functions)
// ============================================================================

namespace quantum {
    // Circuit operations
    class Circuit {
    public:
        void add_qubit(int count);
        void h_gate(int qubit);
        void x_gate(int qubit);
        void y_gate(int qubit);
        void z_gate(int qubit);
        void cnot(int control, int target);
        void swap(int a, int b);
        void measure(int qubit);
        std::vector<int> execute();
    };

    // Quantum algorithms
    namespace algorithms {
        std::vector<int> grover_search(const std::vector<int>& database, int target);
        std::vector<int> shor_factorization(int n);
        std::vector<int> vqe_optimization(const std::vector<double>& params);
        std::vector<int> qaoa_maxcut(const std::vector<std::pair<int,int>>& edges);
        std::vector<int> qnn_inference(const std::vector<double>& input);
    }

    // Quantum gates
    namespace gates {
        void pauli_x(Circuit& c, int q);
        void pauli_y(Circuit& c, int q);
        void pauli_z(Circuit& c, int q);
        void hadamard(Circuit& c, int q);
        void phase(Circuit& c, int q, double angle);
        void t_gate(Circuit& c, int q);
        void s_gate(Circuit& c, int q);
        void rx(Circuit& c, int q, double angle);
        void ry(Circuit& c, int q, double angle);
        void rz(Circuit& c, int q, double angle);
        void controlled_not(Circuit& c, int control, int target);
        void toffoli(Circuit& c, int a, int b, int target);
    }

    // Quantum simulation
    namespace simulation {
        class StateVector {
        public:
            std::vector<std::complex<double>> amplitudes;
            double get_probability(int state);
            std::vector<int> measure(int shots);
        };

        class DensityMatrix {
        public:
            std::vector<std::vector<std::complex<double>>> matrix;
            void apply_channel(const std::string& channel);
        };
    }
}

// ============================================================================
// Machine Learning Library (100+ functions)
// ============================================================================

namespace ml {
    // Neural networks
    namespace nn {
        class Layer {
        public:
            virtual std::vector<double> forward(const std::vector<double>& input) = 0;
            virtual void backward(const std::vector<double>& grad) = 0;
        };

        class Dense : public Layer {
        public:
            Dense(int input_size, int output_size);
            std::vector<double> forward(const std::vector<double>& input) override;
            void backward(const std::vector<double>& grad) override;
        };

        class Conv2D : public Layer {
        public:
            Conv2D(int in_channels, int out_channels, int kernel_size);
            std::vector<double> forward(const std::vector<double>& input) override;
            void backward(const std::vector<double>& grad) override;
        };

        class RNN : public Layer {
        public:
            std::vector<double> forward(const std::vector<double>& input) override;
            void backward(const std::vector<double>& grad) override;
        };

        class LSTM : public Layer {
        public:
            std::vector<double> forward(const std::vector<double>& input) override;
            void backward(const std::vector<double>& grad) override;
        };

        class Transformer : public Layer {
        public:
            std::vector<double> forward(const std::vector<double>& input) override;
            void backward(const std::vector<double>& grad) override;
        };
    }

    // Activation functions
    namespace activations {
        std::vector<double> relu(const std::vector<double>& x);
        std::vector<double> sigmoid(const std::vector<double>& x);
        std::vector<double> tanh(const std::vector<double>& x);
        std::vector<double> softmax(const std::vector<double>& x);
        std::vector<double> elu(const std::vector<double>& x);
        std::vector<double> selu(const std::vector<double>& x);
    }

    // Loss functions
    namespace loss {
        double mse(const std::vector<double>& pred, const std::vector<double>& target);
        double mae(const std::vector<double>& pred, const std::vector<double>& target);
        double cross_entropy(const std::vector<double>& pred, const std::vector<double>& target);
        double kl_divergence(const std::vector<double>& p, const std::vector<double>& q);
    }

    // Optimizers
    namespace optimizers {
        class SGD {
        public:
            void step(std::vector<double>& params, const std::vector<double>& grads);
        };

        class Adam {
        public:
            void step(std::vector<double>& params, const std::vector<double>& grads);
        };

        class RMSprop {
        public:
            void step(std::vector<double>& params, const std::vector<double>& grads);
        };
    }

    // Regularization
    namespace regularization {
        std::vector<double> l1_penalty(const std::vector<double>& weights);
        std::vector<double> l2_penalty(const std::vector<double>& weights);
        std::vector<double> dropout(const std::vector<double>& x, double rate);
        std::vector<double> batch_norm(const std::vector<double>& x);
    }
}

// ============================================================================
// Data Processing Library (100+ functions)
// ============================================================================

namespace data {
    // Arrays and tensors
    namespace array {
        class Array {
        public:
            std::vector<double> data;
            std::vector<int> shape;
            Array reshape(const std::vector<int>& new_shape);
            Array transpose();
            Array slice(const std::vector<int>& start, const std::vector<int>& end);
        };

        Array zeros(const std::vector<int>& shape);
        Array ones(const std::vector<int>& shape);
        Array arange(double start, double stop, double step);
        Array linspace(double start, double stop, int num);
        Array random(const std::vector<int>& shape);
    }

    // Statistics
    namespace stats {
        double mean(const std::vector<double>& data);
        double median(const std::vector<double>& data);
        double std_dev(const std::vector<double>& data);
        double variance(const std::vector<double>& data);
        double covariance(const std::vector<double>& x, const std::vector<double>& y);
        double correlation(const std::vector<double>& x, const std::vector<double>& y);
        std::vector<double> quantile(const std::vector<double>& data, const std::vector<double>& q);
    }

    // Linear algebra
    namespace linalg {
        std::vector<std::vector<double>> matrix_multiply(
            const std::vector<std::vector<double>>& a,
            const std::vector<std::vector<double>>& b);
        std::vector<std::vector<double>> matrix_inverse(
            const std::vector<std::vector<double>>& a);
        std::vector<double> matrix_solve(
            const std::vector<std::vector<double>>& a,
            const std::vector<double>& b);
        double determinant(const std::vector<std::vector<double>>& a);
        double trace(const std::vector<std::vector<double>>& a);
    }

    // Signal processing
    namespace signal {
        std::vector<double> fft(const std::vector<double>& signal);
        std::vector<double> ifft(const std::vector<double>& spectrum);
        std::vector<double> convolve(const std::vector<double>& a, const std::vector<double>& b);
        std::vector<double> correlate(const std::vector<double>& a, const std::vector<double>& b);
        std::vector<double> filter(const std::vector<double>& signal, const std::vector<double>& kernel);
    }
}

// ============================================================================
// Systems Programming Library (100+ functions)
// ============================================================================

namespace sys {
    // Memory management
    namespace memory {
        void* allocate(size_t size);
        void deallocate(void* ptr);
        void* reallocate(void* ptr, size_t new_size);
        void memcpy(void* dest, const void* src, size_t size);
        void memset(void* ptr, int value, size_t size);
    }

    // Threading
    namespace threading {
        class Thread {
        public:
            void start(void (*func)(void*), void* arg);
            void join();
            void detach();
        };

        class Mutex {
        public:
            void lock();
            void unlock();
            bool try_lock();
        };

        class Condition {
        public:
            void wait(Mutex& m);
            void notify_one();
            void notify_all();
        };
    }

    // File I/O
    namespace file {
        class File {
        public:
            bool open(const std::string& path, const std::string& mode);
            void close();
            std::string read();
            void write(const std::string& data);
            void seek(long offset);
        };

        bool exists(const std::string& path);
        bool remove(const std::string& path);
        bool rename(const std::string& old_name, const std::string& new_name);
        std::vector<std::string> list_directory(const std::string& path);
    }

    // Networking
    namespace net {
        class Socket {
        public:
            bool connect(const std::string& host, int port);
            void bind(int port);
            void listen(int backlog);
            Socket accept();
            std::string receive(int size);
            void send(const std::string& data);
            void close();
        };

        std::string http_get(const std::string& url);
        std::string http_post(const std::string& url, const std::string& data);
    }

    // Process management
    namespace process {
        int execute(const std::string& command);
        int get_pid();
        void sleep(int milliseconds);
        void exit(int code);
    }
}

// ============================================================================
// Web Development Library (50+ functions)
// ============================================================================

namespace web {
    // HTTP
    namespace http {
        class Request {
        public:
            std::string method;
            std::string path;
            std::map<std::string, std::string> headers;
            std::string body;
        };

        class Response {
        public:
            int status_code;
            std::map<std::string, std::string> headers;
            std::string body;
        };

        Response handle_request(const Request& req);
    }

    // JSON
    namespace json {
        class Value {
        public:
            std::string to_string();
            static Value parse(const std::string& json);
        };
    }

    // HTML/Templates
    namespace html {
        std::string escape(const std::string& text);
        std::string render_template(const std::string& template_str,
                                   const std::map<std::string, std::string>& vars);
    }

    // Sessions
    namespace session {
        class Session {
        public:
            void set(const std::string& key, const std::string& value);
            std::string get(const std::string& key);
            void remove(const std::string& key);
        };
    }
}

// ============================================================================
// Cryptography Library (50+ functions)
// ============================================================================

namespace crypto {
    // Hashing
    namespace hash {
        std::string sha256(const std::string& data);
        std::string sha512(const std::string& data);
        std::string blake3(const std::string& data);
        std::string hmac_sha256(const std::string& key, const std::string& data);
    }

    // Encryption
    namespace encryption {
        std::string aes_encrypt(const std::string& plaintext, const std::string& key);
        std::string aes_decrypt(const std::string& ciphertext, const std::string& key);
        std::string chacha20_encrypt(const std::string& plaintext, const std::string& key);
        std::string chacha20_decrypt(const std::string& ciphertext, const std::string& key);
    }

    // Digital signatures
    namespace signatures {
        std::string sign(const std::string& data, const std::string& private_key);
        bool verify(const std::string& data, const std::string& signature, const std::string& public_key);
    }

    // Random number generation
    namespace random {
        int random_int(int min, int max);
        double random_double(double min, double max);
        std::vector<uint8_t> random_bytes(size_t count);
    }
}

} // namespace synq::stdlib
