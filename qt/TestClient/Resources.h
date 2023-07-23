#ifndef RESOURCES_H
#define RESOURCES_H

#include "Types.h"
#include "ProgramConfig.h"

class Resources
{
public:
	static ProgramConfig *cfg;
	static QString host;
	static int port;
	static ServerPresetList server_presets;
	static ClientPresetList client_presets;
	static QString users_path;
	static void init(ProgramConfig *config);
	static bool applayHost(const QString &hst, int prt);
	static bool getAvatarHandler(AvatarHandler *hnd, ClientType ctype, int id);
};

#endif // RESOURCES_H
