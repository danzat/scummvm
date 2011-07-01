#include "neverhood/neverhood.h"

#include "common/config-manager.h"
#include "common/error.h"
#include "common/fs.h"

#include "engines/metaengine.h"

static const PlainGameDescriptor neverhood_setting[] = {
    {"neverhood", "Neverhood the bla bla"},
    {0, 0}
};

class NeverhoodMetaEngine : public MetaEngine {
    public:
        virtual const char *getName() const
        {
            return "Neverhood the bla bla";
        }

        virtual const char *getOriginalCopyright() const
        {
            return "Copyright (C) Neverhood, Inc., The";
        }

        virtual GameList getSupportedGames() const
        {
            GameList games;
            const PlainGameDescriptor *g = neverhood_setting;
            while (g->gameid) {
                games.push_back(*g);
                g++;
            }

            return games;
        }

        virtual GameDescriptor findGame(const char *gameid) const
        {
            const PlainGameDescriptor *g = neverhood_setting;
            while (g->gameid) {
                if (0 == scumm_stricmp(gameid, g->gameid))
                    break;
                g++;
            }

            return GameDescriptor(g->gameid, g->description);
        }

        virtual GameList detectGames(const Common::FSList &fslist) const
        {
            GameList detectedGames;

            for (Common::FSList::const_iterator file = fslist.begin(); file != fslist.end(); ++file) {
                if (!file->isDirectory()) {
                    const char *filename = file->getName().c_str();

                    if (0 == scumm_stricmp("c.blb", filename)) {
                        detectedGames.push_back(neverhood_setting[0]);
                        break;
                    }
                }
            }
            
            return detectedGames;
        }

        virtual Common::Error createInstance(OSystem *syst, Engine **engine) const
        {
            assert(syst);
            assert(engine);

            Common::FSList fslist;
            Common::FSNode dir(ConfMan.get("path"));
            if (!dir.getChildren(fslist, Common::FSNode::kListAll)) {
                return Common::kNoGameDataFoundError;
            }

            Common::String gameid = ConfMan.get("gameid");
            GameList detectedGames = detectGames(fslist);

            for (uint i = 0; i < detectedGames.size(); ++i) {
                if (detectedGames[i].gameid() == gameid) {
                    *engine = new Neverhood::NeverhoodEngine(syst);
                    return Common::kNoError;
                }
            }

            return Common::kNoGameDataFoundError;
        }
};

#if PLUGIN_ENABLED_DYNAMIC(NEVERHOOD)
    REGISTER_PLUGIN_DYNAMIC(NEVERHOOD, PLUGIN_TYPE_ENGINE, NeverhoodMetaEngine);
#else
    REGISTER_PLUGIN_STATIC(NEVERHOOD, PLUGIN_TYPE_ENGINE, NeverhoodMetaEngine);
#endif
