
#ifndef _ITLSSPCOMSH
#define _ITLSSPCOMSH
#define CCONV _stdcall
#define NOMANGLE

#ifdef __cplusplus
extern "C"
{
#endif
#include "itl_types.h"
#include "ssp_defines.h"

#define MAX_SSP_PORT 200

#define NO_ENCRYPTION 0
#define ENCRYPTION_SET 1

	typedef struct
	{
		unsigned long long FixedKey;
		unsigned long long EncryptKey;
	} SSP_FULL_KEY;

	typedef struct
	{
		unsigned short packetTime;
		unsigned char PacketLength;
		unsigned char PacketData[255];
	} SSP_PACKET;

	typedef struct
	{
		SSP_FULL_KEY Key;
		unsigned long BaudRate;
		unsigned long Timeout;
		unsigned char PortNumber;
		unsigned char SSPAddress;
		unsigned char RetryLevel;
		unsigned char EncryptionStatus;
		unsigned char CommandDataLength;
		unsigned char CommandData[255];
		unsigned char ResponseStatus;
		unsigned char ResponseDataLength;
		unsigned char ResponseData[255];
		unsigned char IgnoreError;
	} SSP_COMMAND;

	typedef struct
	{
		unsigned char txData[255];
		unsigned char txPtr;
		unsigned char rxData[255];
		unsigned char rxPtr;
		unsigned char txBufferLength;
		unsigned char rxBufferLength;
		unsigned char SSPAddress;
		unsigned char NewResponse;
		unsigned char CheckStuff;
	} SSP_TX_RX_PACKET;

	typedef struct
	{
		long long Generator;
		long long Modulus;
		long long HostInter;
		long long HostRandom;
		long long SlaveInterKey;
		long long SlaveRandom;
		long long KeyHost;
		long long KeySlave;
	} SSP_KEYS;

	/* command status enumeration */
	typedef enum
	{
		PORT_CLOSED,
		PORT_OPEN,
		PORT_ERROR,
		SSP_REPLY_OK,
		SSP_PACKET_ERROR,
		SSP_CMD_TIMEOUT,
	} PORT_STATUS;

	typedef struct
	{
		SSP_FULL_KEY Key;
		unsigned long Timeout;
		unsigned char SSPAddress;
		unsigned char RetryLevel;
		unsigned char EncryptionStatus;
		SSP_PORT port;
	} SSP_COMMAND_SETUP;

	typedef struct
	{
		unsigned char event;
		unsigned long data;
	} SSP_POLL_EVENT;

	typedef struct
	{
		SSP_POLL_EVENT events[20];
		unsigned char event_count;
	} SSP_POLL_DATA;

	typedef struct
	{
		unsigned char UnitType;
		char FirmwareVersion[5];
		char CountryCode[4];
		unsigned long ValueMultiplier;
		unsigned char ProtocolVersion;
	} SSP_UNIT_DATA;

	typedef struct
	{
		unsigned char NumberOfChannels;
		unsigned char ChannelData[16];
	} SSP_CHANNEL_DATA;

	typedef struct
	{
		unsigned char UnitType;
		char FirmwareVersion[5];
		char CountryCode[4];
		unsigned long ValueMultiplier;
		SSP_CHANNEL_DATA ChannelValues;
		SSP_CHANNEL_DATA ChannelSecurity;
		unsigned long RealValueMultiplier;
		unsigned char ProtocolVersion;
	} SSP_SETUP_REQUEST_DATA;

	int SSPSendCommand(const SSP_PORT, SSP_COMMAND *cmd);

	SSP_PORT OpenSSPPort(const char *port);

	void CloseSSPPort(const SSP_PORT port);
	int DownloadFileToTarget(const char *file, const char *port, const unsigned char sspAddress);
	int DownloadDataToTarget(const unsigned char *data, const unsigned long dlength, const char *cPort, const unsigned char sspAddress);
	unsigned long GetDownloadStatus(void);
	int NegotiateSSPEncryption(SSP_PORT port, const char ssp_address, SSP_FULL_KEY *key);

	SSP_RESPONSE_ENUM ssp_setup_encryption(SSP_COMMAND_SETUP *setup, const unsigned long long fixedkey);

	SSP_RESPONSE_ENUM ssp_reset(SSP_COMMAND_SETUP setup);

	SSP_RESPONSE_ENUM ssp_host_protocol(SSP_COMMAND_SETUP setup, const unsigned char host_protocol);

	SSP_RESPONSE_ENUM ssp_poll(SSP_COMMAND_SETUP setup, SSP_POLL_DATA *poll_response);

	SSP_RESPONSE_ENUM ssp_get_serial(SSP_COMMAND_SETUP setup, unsigned long *serial);

	SSP_RESPONSE_ENUM ssp_sync(SSP_COMMAND_SETUP setup);

	SSP_RESPONSE_ENUM ssp_disable(SSP_COMMAND_SETUP setup);

	SSP_RESPONSE_ENUM ssp_enable(SSP_COMMAND_SETUP setup);

	SSP_RESPONSE_ENUM ssp_set_inhibits(SSP_COMMAND_SETUP setup, const unsigned char lowchannels, const unsigned char highchannels);

	SSP_RESPONSE_ENUM ssp_display_on(SSP_COMMAND_SETUP setup);

	SSP_RESPONSE_ENUM ssp_display_off(SSP_COMMAND_SETUP setup);

	SSP_RESPONSE_ENUM ssp_hold_note(SSP_COMMAND_SETUP setup);

	SSP_RESPONSE_ENUM ssp_unit_data(SSP_COMMAND_SETUP setup, SSP_UNIT_DATA *sud);

	SSP_RESPONSE_ENUM ssp_enable_higher_protocol_events(SSP_COMMAND_SETUP setup);

	SSP_RESPONSE_ENUM ssp_channel_value_data(SSP_COMMAND_SETUP setup, SSP_CHANNEL_DATA *scd);

	SSP_RESPONSE_ENUM ssp_channel_security_data(SSP_COMMAND_SETUP setup, SSP_CHANNEL_DATA *scd);

	SSP_RESPONSE_ENUM ssp_last_reject(SSP_COMMAND_SETUP setup, unsigned char *last_reject_reason);

	SSP_RESPONSE_ENUM ssp_setup_request(SSP_COMMAND_SETUP setup, SSP_SETUP_REQUEST_DATA *setup_request_data);
#endif
#ifdef __cplusplus
}
#endif