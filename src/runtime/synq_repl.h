// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_REPL_H
#define SYNQ_REPL_H

namespace synq {

class SynqREPL {
public:
    static void start();
    static void prompt();
    static void stop();

private:
    static bool running;
};

}

#endif // SYNQ_REPL_H