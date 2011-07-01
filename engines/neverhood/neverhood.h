#ifndef NEVERHOOD_H
#define NEVERHOOD_H

#include "common/random.h"
#include "engines/engine.h"
#include "gui/debugger.h"

namespace Neverhood {
    class Console;

    class NeverhoodEngine : public Engine {
        public:
            NeverhoodEngine(OSystem *syst);
            ~NeverhoodEngine();

            virtual Common::Error run();

        private:
            Console *_console;
            Common::RandomSource *_rnd;
    };

    class Console : public GUI::Debugger {
        public:
            Console(NeverhoodEngine *vm) {}
            virtual ~Console(void) {}
    };
} // End of namespace Neverhood

#endif
