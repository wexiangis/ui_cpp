#ifndef __WLAN_H_
#define __WLAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>

//----- 全双工管道封装 -----

typedef struct{
	int fr, fw;
	pid_t pid;
	bool run;
}DuplexPipe;

bool duplex_popen(DuplexPipe *dp, char *cmd);
void duplex_pclose(DuplexPipe *dp);

//----- wlan -----

#include "wpa_ctrl.h"

typedef struct WlanScanInfo{
	char bssid[32];
	int frq;
    int power;
    int keyType;
    char ssid[256];
    struct WlanScanInfo *next;
}WlanScan_Info;

typedef struct{
	char bssid[32];
	int frq;
    // int power;
    int keyType;
    char ssid[256];
	int id;
	char ip[32];
	char addr[32];
	char p2p_dev_addr[32];
	char uuid[128];
	char status;//0/关闭 1/正在连接 2/连接正常
}Wlan_Status;

//wifi 扫描每新增一条网络就回调该函数,由用户自行处理新增的网络
//object/用户数据指针
//example:
//	void wifi_scanCallback(void *object, int num, WlanScan_Info *info);
typedef void (*ScanCallback)(void *object, int num, WlanScan_Info *info);

void wifi_scan(void *object, ScanCallback callback, int timeout);
void wifi_scanStop(void);

int wifi_connect(char *ssid, char *key);
void wifi_disconnect(void);

Wlan_Status *wifi_status(void);
int wifi_signal(int *power);//返回0~100来表示强度  //power:信号强度 例如-33dbm 0表示失败或没有信号

void wifi_exit(void);
void wifi_init(void);

int wifi_through(char *rsp, size_t rspLen, char *cmd, ...);
int wifi_p2p_through(char *rsp, size_t rspLen, char *cmd, ...);
int ap_through(char *rsp, size_t rspLen, char *cmd, ...);

typedef struct WlanApList{
	char addr[32];//接入设备的物理地址
	int time;//连接时长 秒
	struct WlanApList *next;
}WlanAp_List;
//获取接入设备
WlanAp_List *ap_list(int *total);//用完记得释放
void ap_list_release(WlanAp_List *list);
// network_dev : 为热点提供上网源的网络设备,例如 eth0 ppp0
bool ap_start(char *name, char *key, char *network_dev);
void ap_stop();

#ifdef __cplusplus
};
#endif

#endif
