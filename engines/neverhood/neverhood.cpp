#include "common/scummsys.h"

#include "common/config-manager.h"
#include "common/debug.h"
#include "common/debug-channels.h"
#include "common/error.h"
#include "common/EventRecorder.h"
#include "common/file.h"
#include "common/fs.h"

#include "engines/util.h"

#include "neverhood/neverhood.h"

namespace Neverhood {
    NeverhoodEngine::NeverhoodEngine(OSystem *syst)
        : Engine(syst)
    {
        const Common::FSNode gameDataDir(ConfMan.get("path"));
        SearchMan.addSubDirectoryMatching(gameDataDir, "sound");

        _rnd = new Common::RandomSource("Neverhood");

        debug("NeverhoodEngine::NeverhoodEngine");
    }

    NeverhoodEngine::~NeverhoodEngine()
    {
        debug("NeverhoodEngine::~NeverhoodEngine");

        delete _rnd;
    }

    Common::Error NeverhoodEngine::run()
    {
        initGraphics(320, 200, false);

        _console = new Console(this);

        debug("NeverhoodEngine::init");

        return Common::kNoError;
    }
} // End of namespace Neverhood
