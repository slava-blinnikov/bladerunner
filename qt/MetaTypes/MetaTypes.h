#ifndef GLOBALTYPES_H
#define GLOBALTYPES_H

#define PACKET_MAX_SIZE 512
#define PACKET_START_MARKER "#&#&_bladerunner_packet"
#define START_MARKER_LENGTH sizeof(PACKET_START_MARKER)-1

enum ClientType
{
	ClientUnknown	= 0,
	Administrator		= 1,
	Crew			= 2,
	Member			= 3
};

enum ServerQuery
{
	QueryUnknown = 0,
	QueryClientHeartbeat = 1,
	QueryServerHeartbeat = 2,
	QueryAuthorization = 3,
	QueryAuthorizationOk = 4,
	QueryAvatarChangeTime = 5,
	QueryAvatar = 6,
	QueryMembersList = 7,
	QueryMember = 8,
	QueryMembersOnline = 9,
	QueryMemberConnected = 10,
	QueryMemberDisconnected = 11,
	QueryCrewConnected = 12,
	QueryCrewDisconnected = 13,
	QueryStartBeacon = 14,
	QueryStopBeacon = 15,
	QueryBeaconState = 16,
	QueryLocation = 17,
	QueryMemberConfig = 18,
	QuerySetTrackingPermission = 19,
	QuerySetRecordingPermission = 20
};

union Data8Bytes
{
	unsigned long data;
	double fdata;
	unsigned char bytes[8];
};

union Data4Bytes
{
	unsigned int data;
	unsigned char bytes[4];
};

union Data2Bytes
{
	unsigned short data;
	unsigned char bytes[2];
};

unsigned int crc32(unsigned char *buf, unsigned long len, unsigned int *crc_table);
void initCrc32Table(unsigned int *crc_table);
#endif // GLOBALTYPES_H
