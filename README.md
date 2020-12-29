
**开放接口文档**


**简介**：<p>开放接口文档</p>



**Version**:2.1.2

**接口路径**：/v2/api-docs


# agenda

## 新增

**接口描述**:我的申请

**接口地址**:`/api/agenda/add`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"content": "领导讲话",
	"endTime": "11:00",
	"mid": 1,
	"startTime": "09:00",
	"user": "王大锤"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |AgendaAddParam  | AgendaAddParam   |

**schema属性说明**



**AgendaAddParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|content| 内容  | body | false |string  |    |
|endTime| 结束时间  | body | false |string(date-time)  |    |
|mid| 会议id  | body | false |integer(int64)  |    |
|startTime| 开始时间  | body | false |string(date-time)  |    |
|user| 负责人  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议议程

**接口描述**:我的申请,客户端

**接口地址**:`/api/agenda/meeting`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingIdParam  | MeetingIdParam   |

**schema属性说明**



**MeetingIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"content": "",
			"id": 0,
			"time": "",
			"user": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | AgendaMeetingDTO   |
|message|   |string  |    |



**schema属性说明**




**AgendaMeetingDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|content | 内容   |string  |    |
|id | 议程id   |integer(int64)  |    |
|time | 时间   |string  |    |
|user | 负责人   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«AgendaMeetingDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 启用/禁用/删除

**接口描述**:我的申请

**接口地址**:`/api/agenda/modify_status`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1,
	"status": "ENABLED"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |CommonStatusParam  | CommonStatusParam   |

**schema属性说明**



**CommonStatusParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |
|status| 状态( ENABLED(1, "已启用"), DISABLED(0, "已禁用"), DELETED(-1, "已删除");),可用值:ENABLED,DISABLED,DELETED  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 更新

**接口描述**:我的申请

**接口地址**:`/api/agenda/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"agendaId": 1,
	"content": "领导讲话",
	"endTime": "11:00",
	"startTime": "09:00",
	"user": "王大锤"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |AgendaUpdateParam  | AgendaUpdateParam   |

**schema属性说明**



**AgendaUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|agendaId| 议程id  | body | false |integer(int64)  |    |
|content| 内容  | body | false |string  |    |
|endTime| 结束时间  | body | false |string(date-time)  |    |
|startTime| 开始时间  | body | false |string(date-time)  |    |
|user| 负责人  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# applets

## 主持人应用

**接口描述**:应用管理,客户端

**接口地址**:`/api/applets/host_apps`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"otherApps": [
			{
				"code": "",
				"icon": "",
				"id": 0,
				"isHost": false,
				"name": "",
				"url": ""
			}
		],
		"usedApps": [
			{
				"code": "",
				"icon": "",
				"id": 0,
				"isHost": false,
				"name": "",
				"url": ""
			}
		]
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |AppletsAppsDTO  | AppletsAppsDTO   |
|message|   |string  |    |



**schema属性说明**




**AppletsAppsDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|otherApps | 未选中   |array  | AppletsAppsSubDTO   |
|usedApps | 已选中   |array  | AppletsAppsSubDTO   |

**AppletsAppsSubDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|code | 标识   |string  |    |
|icon | 图标   |string  |    |
|id | 主键id   |integer(int32)  |    |
|isHost | 是否主持人应用   |boolean  |    |
|name | 名字   |string  |    |
|url | 路径   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«AppletsAppsDTO»|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 普通用户应用

**接口描述**:应用管理,客户端

**接口地址**:`/api/applets/normal_apps`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"otherApps": [
			{
				"code": "",
				"icon": "",
				"id": 0,
				"isHost": false,
				"name": "",
				"url": ""
			}
		],
		"usedApps": [
			{
				"code": "",
				"icon": "",
				"id": 0,
				"isHost": false,
				"name": "",
				"url": ""
			}
		]
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |AppletsAppsDTO  | AppletsAppsDTO   |
|message|   |string  |    |



**schema属性说明**




**AppletsAppsDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|otherApps | 未选中   |array  | AppletsAppsSubDTO   |
|usedApps | 已选中   |array  | AppletsAppsSubDTO   |

**AppletsAppsSubDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|code | 标识   |string  |    |
|icon | 图标   |string  |    |
|id | 主键id   |integer(int32)  |    |
|isHost | 是否主持人应用   |boolean  |    |
|name | 名字   |string  |    |
|url | 路径   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«AppletsAppsDTO»|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 更新主持人应用

**接口描述**:应用管理

**接口地址**:`/api/applets/update_host_apps`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"otherAppIds": [],
	"usedAppIds": []
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |AppletsUpdateAppsParam  | AppletsUpdateAppsParam   |

**schema属性说明**



**AppletsUpdateAppsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|otherAppIds| 其他appIds, 如：[4,5,6]  | body | false |array  |    |
|usedAppIds| 选中appIds, 如：[1,2,3]  | body | false |array  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 更新普通会议人员应用

**接口描述**:应用管理

**接口地址**:`/api/applets/update_normal_apps`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"otherAppIds": [],
	"usedAppIds": []
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |AppletsUpdateAppsParam  | AppletsUpdateAppsParam   |

**schema属性说明**



**AppletsUpdateAppsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|otherAppIds| 其他appIds, 如：[4,5,6]  | body | false |array  |    |
|usedAppIds| 选中appIds, 如：[1,2,3]  | body | false |array  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# client

## 通信客户端推流、关闭推流、加入同屏、推流确认提示框、会议资料


**接口描述**:


**接口地址**:`/api/client/set_push`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"deviceCode": "deviceCode",
	"typeEnum": "SRS_CLIENT_PUSH_ON"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |ClientSetPushParam  | ClientSetPushParam   |

**schema属性说明**



**ClientSetPushParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|deviceCode| 设备码  | body | true |string  |    |
|typeEnum| 通讯类型: SRS_CLIENT_PUSH_ON, SRS_CLIENT_PUSH_OFF, CLIENT_PLAY_LIVE, CLIENT_PUSH_ON_CONFIRM, CLIENT_TO_FILES,可用值:NORMAL,RESTART,XONDEO_SHUTDOWN_WINDOW,SRS_ON_PUBLISH,SRS_ON_UNPUBLISH,SRS_DISABLED,SRS_ENABLED,SRS_APPLY,SRS_APPLY_DONE,MEETING_CHAT,MEETING_VOTE,MEETING_VOTE_RES,SERIAL_PORT,OPEN_VIDEO,SPEECH_RECOGNITION,OPEN_SUMMARY,VIDEO_PROGRESS,XONDEO_OPEN_WINDOW,PPTVIEW,ROOMCARD_LIGHTCOLOR,BACK_DESKCARD,FILE_SHARE,FILE_NEED,FILE_NEED_RESET,SRS_REPULL,SRS_CLIENT_PUSH_ON,SRS_CLIENT_PUSH_OFF,SHOW_CLIENT,ROOMCARD_RELOAD_SIGN,CLIENT_PLAY_LIVE,SHOW_CLIENT_NO_CLOSEPPT,CLIENT_PUSH_ON_CONFIRM,CLIENT_TO_FILES,CLIENT_UPDATE,CLIENT_UPDATE_ASAR  | body | true |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 前置客户端窗体


**接口描述**:


**接口地址**:`/api/client/show`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"deviceCode": "deviceCode",
	"isClosePPT": true,
	"isClosePlayer": true
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |ClientShowParam  | ClientShowParam   |

**schema属性说明**



**ClientShowParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|deviceCode| 设备码  | body | true |string  |    |
|isClosePPT| 同时关闭PPT  | body | false |boolean  |    |
|isClosePlayer| 关闭同屏  | body | false |boolean  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# count

## 统计数据总览

**接口描述**:会议统计

**接口地址**:`/api/count/info`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"allMeetingCount": 0,
		"allMeetingUserCount": 0,
		"newMeetingCount": 0,
		"todayMeetingCount": 0,
		"todayMeetingUserCount": 0
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |CountInfosDTO  | CountInfosDTO   |
|message|   |string  |    |



**schema属性说明**




**CountInfosDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|allMeetingCount | 累计会议   |integer(int64)  |    |
|allMeetingUserCount | 累计会议人次   |integer(int64)  |    |
|newMeetingCount | 新增会议(今日)   |integer(int32)  |    |
|todayMeetingCount | 当日会议   |integer(int32)  |    |
|todayMeetingUserCount | 当日会议人次   |integer(int32)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«CountInfosDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 分类会议召开次数、占比

**接口描述**:会议统计

**接口地址**:`/api/count/meeting_type`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"endTime": "2020-12-31 23:59",
	"startTime": "2020-01-01 00:00"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |CountMeetingTypeParam  | CountMeetingTypeParam   |

**schema属性说明**



**CountMeetingTypeParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|endTime| 结束时间  | body | false |string(date-time)  |    |
|startTime| 开始时间  | body | false |string(date-time)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"count": 0,
			"meetingTypeId": 0,
			"name": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | CountMeetingTypeDTO   |
|message|   |string  |    |



**schema属性说明**




**CountMeetingTypeDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|count | 会议分类召开次数   |integer(int64)  |    |
|meetingTypeId | 会议分类id   |integer(int64)  |    |
|name | 会议分类名称   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«CountMeetingTypeDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议室使用占比

**接口描述**:会议统计

**接口地址**:`/api/count/room_percent`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"roomclasses": [
			{
				"classId": 0,
				"classTitle": "",
				"useCount": 0
			}
		],
		"rooms": [
			{
				"classTitle": "",
				"roomId": 0,
				"title": "",
				"useCount": 0
			}
		]
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |CountRoomPercentDTO  | CountRoomPercentDTO   |
|message|   |string  |    |



**schema属性说明**




**CountRoomPercentDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|roomclasses | 分区使用情况   |array  | CountRoomPercent_RC_DTO   |
|rooms | 单个会议室使用情况   |array  | CountRoomPercent_R_DTO   |

**CountRoomPercent_RC_DTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|classId | 分区id   |integer(int64)  |    |
|classTitle | 分区名称   |string  |    |
|useCount | 使用次数   |integer(int64)  |    |

**CountRoomPercent_R_DTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|classTitle | 分区名称   |string  |    |
|roomId | 会议室id   |integer(int64)  |    |
|title | 会议室名称   |string  |    |
|useCount | 使用次数   |integer(int64)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«CountRoomPercentDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 每日会议场次、人次报表

**接口描述**:会议统计

**接口地址**:`/api/count/time_date`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"endTime": "2020-01-01 00:00",
	"startTime": "2018-01-01 00:00"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |CountTimeDateParam  | CountTimeDateParam   |

**schema属性说明**



**CountTimeDateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|endTime| 结束时间  | body | false |string(date-time)  |    |
|startTime| 开始时间  | body | false |string(date-time)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"m_count": 0,
			"mu_count": 0,
			"time": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | CountTimeDateDTO   |
|message|   |string  |    |



**schema属性说明**




**CountTimeDateDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|m_count | 当日会议场次   |integer(int64)  |    |
|mu_count | 当日会议人次   |integer(int64)  |    |
|time | 时间   |string(date-time)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«CountTimeDateDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# department

## 新增

**接口描述**:部门管理

**接口地址**:`/api/department/add`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"name": "开发部",
	"remark": "备注"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |DepartmentAddParam  | DepartmentAddParam   |

**schema属性说明**



**DepartmentAddParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|name| 部门名称  | body | false |string  |    |
|remark| 备注  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 所有部门

**接口描述**:部门管理

**接口地址**:`/api/department/all`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"gmtCreate": "",
			"gmtModify": "",
			"id": 0,
			"name": "",
			"remark": "",
			"status": "",
			"statusEnum": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | DepartmentAllDTO   |
|message|   |string  |    |



**schema属性说明**




**DepartmentAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|gmtCreate | 创建时间   |string(date-time)  |    |
|gmtModify | 最后编辑   |string(date-time)  |    |
|id | 部门id   |integer(int64)  |    |
|name | 部门名称   |string  |    |
|remark | 备注   |string  |    |
|status | 状态   |string  |    |
|statusEnum | 状态,可用值:ENABLED,DISABLED,DELETED   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«DepartmentAllDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 已启用的部门

**接口描述**:部门管理,我的申请,会议列表,会议管理,会议查询

**接口地址**:`/api/department/enableds`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"gmtCreate": "",
			"gmtModify": "",
			"id": 0,
			"name": "",
			"remark": "",
			"status": "",
			"statusEnum": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | DepartmentAllDTO   |
|message|   |string  |    |



**schema属性说明**




**DepartmentAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|gmtCreate | 创建时间   |string(date-time)  |    |
|gmtModify | 最后编辑   |string(date-time)  |    |
|id | 部门id   |integer(int64)  |    |
|name | 部门名称   |string  |    |
|remark | 备注   |string  |    |
|status | 状态   |string  |    |
|statusEnum | 状态,可用值:ENABLED,DISABLED,DELETED   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«DepartmentAllDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 启用/禁用/删除

**接口描述**:部门管理

**接口地址**:`/api/department/modify_status`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1,
	"status": "ENABLED"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |CommonStatusParam  | CommonStatusParam   |

**schema属性说明**



**CommonStatusParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |
|status| 状态( ENABLED(1, "已启用"), DISABLED(0, "已禁用"), DELETED(-1, "已删除");),可用值:ENABLED,DISABLED,DELETED  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 更新

**接口描述**:部门管理

**接口地址**:`/api/department/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1,
	"name": "开发部",
	"remark": "备注"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |DepartmentUpdateParam  | DepartmentUpdateParam   |

**schema属性说明**



**DepartmentUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 部门id  | body | false |integer(int64)  |    |
|name| 部门名称  | body | false |string  |    |
|remark| 备注  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# device

## 新增虚拟设备


**接口描述**:


**接口地址**:`/api/device/add_uuid_device`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 大屏绑定会议室

**接口描述**:设备列表

**接口地址**:`/api/device/bigscreen_bind_room`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"deviceId": 1,
	"roomId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |DeviceBigscreenBindRoomParam  | DeviceBigscreenBindRoomParam   |

**schema属性说明**



**DeviceBigscreenBindRoomParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|deviceId| 大屏设备id  | body | false |integer(int64)  |    |
|roomId| 会议室id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 外置桌牌绑定设备

**接口描述**:设备列表

**接口地址**:`/api/device/bind`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"bindDeviceId": 2,
	"deviceId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |DeviceBindParam  | DeviceBindParam   |

**schema属性说明**



**DeviceBindParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|bindDeviceId| 绑定客户端设备id  | body | false |integer(int64)  |    |
|deviceId| 外置桌牌设备id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取可绑定的客户端设备——客户端（外置桌牌）

**接口描述**:设备列表

**接口地址**:`/api/device/bind_devices`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"keyword": ""
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |DeviceBindDevicesParam  | DeviceBindDevicesParam   |

**schema属性说明**



**DeviceBindDevicesParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|keyword| 关键字，number/code  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"bindId": 0,
			"code": "",
			"deviceId": 0,
			"lastLogin": "",
			"number": 0,
			"roomId": 0,
			"roomName": "",
			"status": "",
			"statusEnum": "",
			"type": "",
			"typeEnum": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | DeviceListDTO   |
|message|   |string  |    |



**schema属性说明**




**DeviceListDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|bindId | 绑定设备id   |integer(int64)  |    |
|code | 设备标识   |string  |    |
|deviceId | 设备id   |integer(int64)  |    |
|lastLogin | 最后登录   |string(date-time)  |    |
|number | 设备号   |integer(int32)  |    |
|roomId | 会议室id   |integer(int64)  |    |
|roomName | 会议室名称   |string  |    |
|status | 设备状态   |string  |    |
|statusEnum | 设备状态,可用值:ENABLED,DISABLED,DELETED   |string  |    |
|type | 设备类型   |string  |    |
|typeEnum | 设备类型<br/>     CLIENT_DESKCARD(11, "客户端自带桌牌"),<br/>    CLIENT_ONLY(12, "单独客户端"),<br/>    DESKCARD_ONLY(13, "单独桌牌"),<br/>    CLIENT_DESKCARD_WITHOUT_DESKCARD(14, "客户端（外置桌牌）"),<br/>    CLIENT_DESKCARD_WITHOUT_CLIENT(15, "外置桌牌"),<br/>    MOBILE_CLIENT(16, "移动客户端"),<br/>    BIG_SCREEN(17, "大屏"),<br/>    ROOMCARD(21, "门牌"),<br/>    WEB(31, "web"),可用值:CLIENT_DESKCARD,CLIENT_ONLY,DESKCARD_ONLY,CLIENT_DESKCARD_WITHOUT_DESKCARD,CLIENT_DESKCARD_WITHOUT_CLIENT,MOBILE_CLIENT,BIG_SCREEN,ROOMCARD,WEB   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«DeviceListDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 根据设备id查找设备

**接口描述**:设备列表

**接口地址**:`/api/device/find_by_id`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"bindId": 0,
		"code": "",
		"deviceId": 0,
		"lastLogin": "",
		"number": 0,
		"roomId": 0,
		"roomName": "",
		"status": "",
		"statusEnum": "",
		"type": "",
		"typeEnum": ""
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |DeviceListDTO  | DeviceListDTO   |
|message|   |string  |    |



**schema属性说明**




**DeviceListDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|bindId | 绑定设备id   |integer(int64)  |    |
|code | 设备标识   |string  |    |
|deviceId | 设备id   |integer(int64)  |    |
|lastLogin | 最后登录   |string(date-time)  |    |
|number | 设备号   |integer(int32)  |    |
|roomId | 会议室id   |integer(int64)  |    |
|roomName | 会议室名称   |string  |    |
|status | 设备状态   |string  |    |
|statusEnum | 设备状态,可用值:ENABLED,DISABLED,DELETED   |string  |    |
|type | 设备类型   |string  |    |
|typeEnum | 设备类型<br/>     CLIENT_DESKCARD(11, "客户端自带桌牌"),<br/>    CLIENT_ONLY(12, "单独客户端"),<br/>    DESKCARD_ONLY(13, "单独桌牌"),<br/>    CLIENT_DESKCARD_WITHOUT_DESKCARD(14, "客户端（外置桌牌）"),<br/>    CLIENT_DESKCARD_WITHOUT_CLIENT(15, "外置桌牌"),<br/>    MOBILE_CLIENT(16, "移动客户端"),<br/>    BIG_SCREEN(17, "大屏"),<br/>    ROOMCARD(21, "门牌"),<br/>    WEB(31, "web"),可用值:CLIENT_DESKCARD,CLIENT_ONLY,DESKCARD_ONLY,CLIENT_DESKCARD_WITHOUT_DESKCARD,CLIENT_DESKCARD_WITHOUT_CLIENT,MOBILE_CLIENT,BIG_SCREEN,ROOMCARD,WEB   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«DeviceListDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 导出桌牌excel


**接口描述**:


**接口地址**:`/api/device/get_excel`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["application/octet-stream"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json

```

**响应参数**:


暂无





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 根据deviceCode获取Mid


**接口描述**:


**接口地址**:`/api/device/get_mid`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|deviceCode| 设备唯一标识  | query | true |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"mid": 0
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |DeviceGetMidDTO  | DeviceGetMidDTO   |
|message|   |string  |    |



**schema属性说明**




**DeviceGetMidDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|mid |    |integer(int64)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«DeviceGetMidDTO»|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 设备列表

**接口描述**:设备列表

**接口地址**:`/api/device/list`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"types": []
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |DeviceListParam  | DeviceListParam   |

**schema属性说明**



**DeviceListParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|types| 设备类型,可用值:CLIENT_DESKCARD,CLIENT_ONLY,DESKCARD_ONLY,CLIENT_DESKCARD_WITHOUT_DESKCARD,CLIENT_DESKCARD_WITHOUT_CLIENT,MOBILE_CLIENT,BIG_SCREEN,ROOMCARD,WEB  | body | false |array  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"bindId": 0,
			"code": "",
			"deviceId": 0,
			"lastLogin": "",
			"number": 0,
			"roomId": 0,
			"roomName": "",
			"status": "",
			"statusEnum": "",
			"type": "",
			"typeEnum": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | DeviceListDTO   |
|message|   |string  |    |



**schema属性说明**




**DeviceListDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|bindId | 绑定设备id   |integer(int64)  |    |
|code | 设备标识   |string  |    |
|deviceId | 设备id   |integer(int64)  |    |
|lastLogin | 最后登录   |string(date-time)  |    |
|number | 设备号   |integer(int32)  |    |
|roomId | 会议室id   |integer(int64)  |    |
|roomName | 会议室名称   |string  |    |
|status | 设备状态   |string  |    |
|statusEnum | 设备状态,可用值:ENABLED,DISABLED,DELETED   |string  |    |
|type | 设备类型   |string  |    |
|typeEnum | 设备类型<br/>     CLIENT_DESKCARD(11, "客户端自带桌牌"),<br/>    CLIENT_ONLY(12, "单独客户端"),<br/>    DESKCARD_ONLY(13, "单独桌牌"),<br/>    CLIENT_DESKCARD_WITHOUT_DESKCARD(14, "客户端（外置桌牌）"),<br/>    CLIENT_DESKCARD_WITHOUT_CLIENT(15, "外置桌牌"),<br/>    MOBILE_CLIENT(16, "移动客户端"),<br/>    BIG_SCREEN(17, "大屏"),<br/>    ROOMCARD(21, "门牌"),<br/>    WEB(31, "web"),可用值:CLIENT_DESKCARD,CLIENT_ONLY,DESKCARD_ONLY,CLIENT_DESKCARD_WITHOUT_DESKCARD,CLIENT_DESKCARD_WITHOUT_CLIENT,MOBILE_CLIENT,BIG_SCREEN,ROOMCARD,WEB   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«DeviceListDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 启用/禁用/删除

**接口描述**:设备列表

**接口地址**:`/api/device/modify_status`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1,
	"status": "ENABLED"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |CommonStatusParam  | CommonStatusParam   |

**schema属性说明**



**CommonStatusParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |
|status| 状态( ENABLED(1, "已启用"), DISABLED(0, "已禁用"), DELETED(-1, "已删除");),可用值:ENABLED,DISABLED,DELETED  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 查找设备

**接口描述**:设备列表

**接口地址**:`/api/device/search`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"keyword": "keyword"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |DeviceSearchParam  | DeviceSearchParam   |

**schema属性说明**



**DeviceSearchParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|keyword| 搜索number/code  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"bindId": 0,
			"code": "",
			"deviceId": 0,
			"lastLogin": "",
			"number": 0,
			"roomId": 0,
			"roomName": "",
			"status": "",
			"statusEnum": "",
			"type": "",
			"typeEnum": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | DeviceListDTO   |
|message|   |string  |    |



**schema属性说明**




**DeviceListDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|bindId | 绑定设备id   |integer(int64)  |    |
|code | 设备标识   |string  |    |
|deviceId | 设备id   |integer(int64)  |    |
|lastLogin | 最后登录   |string(date-time)  |    |
|number | 设备号   |integer(int32)  |    |
|roomId | 会议室id   |integer(int64)  |    |
|roomName | 会议室名称   |string  |    |
|status | 设备状态   |string  |    |
|statusEnum | 设备状态,可用值:ENABLED,DISABLED,DELETED   |string  |    |
|type | 设备类型   |string  |    |
|typeEnum | 设备类型<br/>     CLIENT_DESKCARD(11, "客户端自带桌牌"),<br/>    CLIENT_ONLY(12, "单独客户端"),<br/>    DESKCARD_ONLY(13, "单独桌牌"),<br/>    CLIENT_DESKCARD_WITHOUT_DESKCARD(14, "客户端（外置桌牌）"),<br/>    CLIENT_DESKCARD_WITHOUT_CLIENT(15, "外置桌牌"),<br/>    MOBILE_CLIENT(16, "移动客户端"),<br/>    BIG_SCREEN(17, "大屏"),<br/>    ROOMCARD(21, "门牌"),<br/>    WEB(31, "web"),可用值:CLIENT_DESKCARD,CLIENT_ONLY,DESKCARD_ONLY,CLIENT_DESKCARD_WITHOUT_DESKCARD,CLIENT_DESKCARD_WITHOUT_CLIENT,MOBILE_CLIENT,BIG_SCREEN,ROOMCARD,WEB   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«DeviceListDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 发送组播socket


**接口描述**:


**接口地址**:`/api/device/send_multicast_socket`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 导入桌牌excel


**接口描述**:


**接口地址**:`/api/device/set_excel`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 设备编辑

**接口描述**:设备列表,客户端

**接口地址**:`/api/device/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"deviceId": 1,
	"number": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |DeviceUpdateParam  | DeviceUpdateParam   |

**schema属性说明**



**DeviceUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|deviceId| 设备id  | body | false |integer(int64)  |    |
|number| 设备编号  | body | false |integer(int32)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 更新桌牌物理信息（客户端自带桌牌类型）


**接口描述**:


**接口地址**:`/api/device/update_screen`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"deviceId": 1,
	"screenHeight": 600,
	"screenImgExt": "jpg",
	"screenPort": 8088,
	"screenWidth": 1024
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |DeviceUpdateScreenParam  | DeviceUpdateScreenParam   |

**schema属性说明**



**DeviceUpdateScreenParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|deviceId| 设备id  | body | false |integer(int64)  |    |
|screenHeight| 桌牌高度  | body | false |integer(int32)  |    |
|screenImgExt| 桌牌支持的图片后缀  | body | false |string  |    |
|screenPort| 桌牌socket端口号  | body | false |integer(int32)  |    |
|screenWidth| 桌牌宽度  | body | false |integer(int32)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# file

## 新增议题材料

**接口描述**:我的申请,客户端

**接口地址**:`/api/file/add`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |
|issueId| issueId  | query | true |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 新增议题材料-ftp


**接口描述**:


**接口地址**:`/api/file/add_ftp`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"issueId": 0,
	"oriName": "",
	"uid": 0,
	"url": ""
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |FileAddFtpParam  | FileAddFtpParam   |

**schema属性说明**



**FileAddFtpParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|issueId| 议题材料  | body | false |integer(int64)  |    |
|oriName| 文件名称  | body | false |string  |    |
|uid| 用户id  | body | false |integer(int64)  |    |
|url| 文件路径  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 客户端历史文件搜索


**接口描述**:


**接口地址**:`/api/file/client_search`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"name": "文件名称",
	"pageNum": 1,
	"pageSize": 10
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |FileSearchParam  | FileSearchParam   |

**schema属性说明**



**FileSearchParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|name| 文件名称  | body | false |string  |    |
|pageNum| 当前页号  | body | false |integer(int32)  |    |
|pageSize| 每页记录数  | body | false |integer(int32)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"hasNext": true,
		"list": [
			{
				"commentPdf": "",
				"edit": "",
				"ext": "",
				"gmtCreate": "",
				"id": 0,
				"name": "",
				"size": "",
				"uploadUser": "",
				"url": "",
				"view": ""
			}
		],
		"pageNum": 0,
		"pageSize": 0,
		"totalCount": 0,
		"totalPage": 0
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |PageDTO«FileListDTO»  | PageDTO«FileListDTO»   |
|message|   |string  |    |



**schema属性说明**




**PageDTO«FileListDTO»**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|hasNext |    |boolean  |    |
|list |    |array  | FileListDTO   |
|pageNum |    |integer(int32)  |    |
|pageSize |    |integer(int32)  |    |
|totalCount |    |integer(int32)  |    |
|totalPage |    |integer(int32)  |    |

**FileListDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|commentPdf | 批注用pdf   |string  |    |
|edit | 编辑url   |string  |    |
|ext | 扩展名   |string  |    |
|gmtCreate | 上传时间   |string(date-time)  |    |
|id | 文件id   |integer(int64)  |    |
|name | 文件名称   |string  |    |
|size | 文件大小   |string  |    |
|uploadUser | 上传人   |string  |    |
|url | 文件url   |string  |    |
|view | 查看url   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«PageDTO«FileListDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 删除

**接口描述**:我的申请

**接口地址**:`/api/file/delete`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 分片下载pdf文件


**接口描述**:


**接口地址**:`/api/file/downfile`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["application/octet-stream"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|url| url  | query | false |string  |    |

**响应示例**:

```json

```

**响应参数**:


暂无





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 下载文件


**接口描述**:


**接口地址**:`/api/file/download/{fileId}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["application/octet-stream"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|fileId| fileId  | path | true |integer  |    |

**响应示例**:

```json

```

**响应参数**:


暂无





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 覆盖议题材料

**接口描述**:我的申请,客户端

**接口地址**:`/api/file/edit`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |
|fileId| fileId  | query | true |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 文件上下移

**接口描述**:我的申请

**接口地址**:`/api/file/exchange_sort`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"firstId": 1,
	"secondId": 2
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |CommonExchangeSortParam  | CommonExchangeSortParam   |

**schema属性说明**



**CommonExchangeSortParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|firstId| 第一个主键id  | body | false |integer(int64)  |    |
|secondId| 第二个主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 议题材料列表

**接口描述**:我的申请,会议列表,会议管理,会议查询,文件归档,客户端

**接口地址**:`/api/file/list`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"issueId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |FileListParam  | FileListParam   |

**schema属性说明**



**FileListParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|issueId| 议题id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"commentPdf": "",
			"edit": "",
			"ext": "",
			"gmtCreate": "",
			"id": 0,
			"name": "",
			"size": "",
			"uploadUser": "",
			"url": "",
			"view": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | FileListDTO   |
|message|   |string  |    |



**schema属性说明**




**FileListDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|commentPdf | 批注用pdf   |string  |    |
|edit | 编辑url   |string  |    |
|ext | 扩展名   |string  |    |
|gmtCreate | 上传时间   |string(date-time)  |    |
|id | 文件id   |integer(int64)  |    |
|name | 文件名称   |string  |    |
|size | 文件大小   |string  |    |
|uploadUser | 上传人   |string  |    |
|url | 文件url   |string  |    |
|view | 查看url   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«FileListDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议材料列表_前端下载


**接口描述**:


**接口地址**:`/api/file/meeting_files/{mid}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|mid| mid  | path | true |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"ext": "",
			"name": "",
			"url": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | FileMeetingFilesDTO   |
|message|   |string  |    |



**schema属性说明**




**FileMeetingFilesDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|ext | 扩展名   |string  |    |
|name | 文件名称（无后缀）   |string  |    |
|url | 文件url   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«FileMeetingFilesDTO»»|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议材料列表

**接口描述**:会议查询,文件归档

**接口地址**:`/api/file/meetinglist`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingIdParam  | MeetingIdParam   |

**schema属性说明**



**MeetingIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"ext": "",
			"gmtCreate": "",
			"id": 0,
			"issueId": 0,
			"name": "",
			"size": "",
			"uploadUser": "",
			"url": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | FileMeetingListDTO   |
|message|   |string  |    |



**schema属性说明**




**FileMeetingListDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|ext | 扩展名   |string  |    |
|gmtCreate | 上传时间   |string(date-time)  |    |
|id | 文件id   |integer(int64)  |    |
|issueId | 议题id   |integer(int64)  |    |
|name | 文件名称   |string  |    |
|size | 文件大小   |string  |    |
|uploadUser | 上传人   |string  |    |
|url | 文件url   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«FileMeetingListDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 材料查看权限列表

**接口描述**:我的申请,会议列表,会议管理

**接口地址**:`/api/file/muids`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"departmentId": 0,
	"fileId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |FileMuidsParam  | FileMuidsParam   |

**schema属性说明**



**FileMuidsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|departmentId| 部门id，'0'代表全部  | body | false |integer(int64)  |    |
|fileId| 材料id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"isCanRead": false,
			"muid": 0,
			"name": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | FileMuidsDTO   |
|message|   |string  |    |



**schema属性说明**




**FileMuidsDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|isCanRead | 是否能查看   |boolean  |    |
|muid | 会议人员id   |integer(int64)  |    |
|name | 姓名   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«FileMuidsDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 文件搜索

**接口描述**:会议查询,文件归档

**接口地址**:`/api/file/search`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"name": "文件名称",
	"pageNum": 1,
	"pageSize": 10
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |FileSearchParam  | FileSearchParam   |

**schema属性说明**



**FileSearchParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|name| 文件名称  | body | false |string  |    |
|pageNum| 当前页号  | body | false |integer(int32)  |    |
|pageSize| 每页记录数  | body | false |integer(int32)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"hasNext": true,
		"list": [
			{
				"gmtCreate": "",
				"id": 0,
				"name": "",
				"size": "",
				"uploadUser": "",
				"url": ""
			}
		],
		"pageNum": 0,
		"pageSize": 0,
		"totalCount": 0,
		"totalPage": 0
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |PageDTO«FileSearchDTO»  | PageDTO«FileSearchDTO»   |
|message|   |string  |    |



**schema属性说明**




**PageDTO«FileSearchDTO»**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|hasNext |    |boolean  |    |
|list |    |array  | FileSearchDTO   |
|pageNum |    |integer(int32)  |    |
|pageSize |    |integer(int32)  |    |
|totalCount |    |integer(int32)  |    |
|totalPage |    |integer(int32)  |    |

**FileSearchDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|gmtCreate | 上传时间   |string(date-time)  |    |
|id | 文件id   |integer(int64)  |    |
|name | 文件名称   |string  |    |
|size | 文件大小   |string  |    |
|uploadUser | 上传人   |string  |    |
|url | 文件url   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«PageDTO«FileSearchDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 补充会后资料

**接口描述**:文件归档

**接口地址**:`/api/file/supplement`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |
|mid| mid  | query | false |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 编辑材料查看权限

**接口描述**:我的申请,会议列表,会议管理

**接口地址**:`/api/file/update_muids`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"datas": [
		{
			"isCanRead": false,
			"muid": 1
		}
	],
	"fileId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |FileUpdateMuidsParam  | FileUpdateMuidsParam   |

**schema属性说明**



**FileUpdateMuidsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|datas| 查看权限List  | body | false |array  | FileUpdateMuidsSubParam   |
|fileId| 材料id  | body | false |integer(int64)  |    |

**FileUpdateMuidsSubParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|isCanRead| 是否能查看  | body | false |boolean  |    |
|muid| 会议人员id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## word2pdf回传文件


**接口描述**:


**接口地址**:`/api/file/upload_absfile`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |
|save| save  | query | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# file_comment

## 新增或更新批注

**接口描述**:客户端

**接口地址**:`/api/file_comment/add_or_update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"base64": "",
	"fileId": 1,
	"num": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |FileCommentAddOrUpdateParam  | FileCommentAddOrUpdateParam   |

**schema属性说明**



**FileCommentAddOrUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|base64| png图片(base64)  | body | false |string  |    |
|fileId| 材料id  | body | false |integer(int64)  |    |
|num| 页码  | body | false |integer(int32)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 新增或更新批注(笔迹)

**接口描述**:客户端

**接口地址**:`/api/file_comment/add_or_update_data`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"data": "",
	"fileId": 1,
	"num": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |FileCommentAddOrUpdateDataParam  | FileCommentAddOrUpdateDataParam   |

**schema属性说明**



**FileCommentAddOrUpdateDataParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|data| 操作data  | body | false |string  |    |
|fileId| 材料id  | body | false |integer(int64)  |    |
|num| 页码  | body | false |integer(int32)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 查看批注


**接口描述**:


**接口地址**:`/api/file_comment/{fileId}/muid/{muid}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|fileId| fileId  | path | true |integer  |    |
|muid| muid  | path | true |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"num": 0,
			"url": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | FileCommentAllDTO   |
|message|   |string  |    |



**schema属性说明**




**FileCommentAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|num | 页码   |integer(int32)  |    |
|url | 图片路径   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«FileCommentAllDTO»»|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 查看批注(笔迹)


**接口描述**:


**接口地址**:`/api/file_comment/{fileId}/muid_data/{muid}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|fileId| fileId  | path | true |integer  |    |
|muid| muid  | path | true |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"data": "",
			"num": 0
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | FileCommentAllDataDTO   |
|message|   |string  |    |



**schema属性说明**




**FileCommentAllDataDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|data | 笔迹data   |string  |    |
|num | 页码   |integer(int32)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«FileCommentAllDataDTO»»|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# goods

## 新增会议物资

**接口描述**:会议物资

**接口地址**:`/api/goods/add`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"name": "投影仪",
	"pid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |GoodsAddParam  | GoodsAddParam   |

**schema属性说明**



**GoodsAddParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|name| 物资名称  | body | false |string  |    |
|pid| 上级物资id,首级为0  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 查询所有物资


**接口描述**:


**接口地址**:`/api/goods/all`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"children": [
				{
					"children": [
						{}
					],
					"goodsId": 0,
					"name": "",
					"pid": 0
				}
			],
			"goodsId": 0,
			"name": "",
			"pid": 0
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | GoodsAllDTO   |
|message|   |string  |    |



**schema属性说明**




**GoodsAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|children | 子项   |array  | GoodsAllDTO   |
|goodsId | 物资id   |integer(int64)  |    |
|name | 名称   |string  |    |
|pid | 上级id   |integer(int64)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«GoodsAllDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 删除

**接口描述**:会议物资

**接口地址**:`/api/goods/delete`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"goodsId": 0
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |GoodsIdParam  | GoodsIdParam   |

**schema属性说明**



**GoodsIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|goodsId| 物资id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 查询会议物资需求选中情况


**接口描述**:


**接口地址**:`/api/goods/meeting`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingIdParam  | MeetingIdParam   |

**schema属性说明**



**MeetingIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"children": [
				{
					"children": [
						{}
					],
					"goodsId": 0,
					"isSelected": false,
					"name": "",
					"pid": 0
				}
			],
			"goodsId": 0,
			"isSelected": false,
			"name": "",
			"pid": 0
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | GoodsMeetingDTO   |
|message|   |string  |    |



**schema属性说明**




**GoodsMeetingDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|children | 子项   |array  | GoodsMeetingDTO   |
|goodsId | 物资id   |integer(int64)  |    |
|isSelected | 是否选中   |boolean  |    |
|name | 名称   |string  |    |
|pid | 上级id   |integer(int64)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«GoodsMeetingDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 设置会议物资需求选中情况


**接口描述**:


**接口地址**:`/api/goods/meeting_update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"goodsIds": [],
	"mid": 0
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |GoodsMeetingUpdateParam  | GoodsMeetingUpdateParam   |

**schema属性说明**



**GoodsMeetingUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|goodsIds| 物资id数组  | body | false |array  |    |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 编辑

**接口描述**:会议物资

**接口地址**:`/api/goods/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"goodsId": 1,
	"name": "投影仪"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |GoodsUpdateParam  | GoodsUpdateParam   |

**schema属性说明**



**GoodsUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|goodsId| 物资id  | body | false |integer(int64)  |    |
|name| 物资名称  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# home_page

## 新增首页模板

**接口描述**:首页模板

**接口地址**:`/api/home_page/add`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 新增桌牌模板

**接口描述**:桌牌模板

**接口地址**:`/api/home_page/add_deskcard`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取首页模板列表

**接口描述**:首页模板

**接口地址**:`/api/home_page/all`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"backgroundImg": "setting/homepage/homepage.png",
			"id": 0,
			"isSelected": false,
			"nameSetting": {
				"color": "0xfdde4e",
				"fontsize_width_percent": 0.11,
				"horizontal_center_percent": 0.5,
				"isBold": false,
				"isItalic": false,
				"isShow": true,
				"isUnderline": false,
				"title": "与会者",
				"typeface": "楷体",
				"vertical_center_percent": 0.44
			},
			"postSetting": {
				"color": "0xfdde4e",
				"fontsize_width_percent": 0.05,
				"horizontal_center_percent": 0.75,
				"isBold": false,
				"isItalic": false,
				"isShow": true,
				"isUnderline": false,
				"title": "职位",
				"typeface": "楷体",
				"vertical_center_percent": 0.75
			},
			"screenShoot": "",
			"seatSetting": {
				"color": "0xfdde4e",
				"fontsize_width_percent": 0.05,
				"horizontal_center_percent": 0.3,
				"isBold": false,
				"isItalic": false,
				"isShow": true,
				"isUnderline": false,
				"title": "座位号",
				"typeface": "楷体",
				"vertical_center_percent": 0.75
			},
			"title": "",
			"titleSetting": {
				"color": "0xfdde4e",
				"fontsize_width_percent": 0.04,
				"horizontal_center_percent": 0.5,
				"isBold": false,
				"isItalic": false,
				"isShow": true,
				"isUnderline": false,
				"title": "会议标题",
				"typeface": "楷体",
				"vertical_center_percent": 0.2
			}
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | HomePageAllDTO   |
|message|   |string  |    |



**schema属性说明**




**HomePageAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|backgroundImg | 模板背景图片   |string  |    |
|id | 首页模板id   |integer(int64)  |    |
|isSelected | 是否当前模板   |boolean  |    |
|nameSetting | 姓名设置   |HomePageUpdateNameParam  | HomePageUpdateNameParam   |
|postSetting | 职位设置   |HomePageUpdatePostParam  | HomePageUpdatePostParam   |
|screenShoot | 缩略图   |string  |    |
|seatSetting | 座位设置   |HomePageUpdateSeatParam  | HomePageUpdateSeatParam   |
|title | 首页模板标题   |string  |    |
|titleSetting | 标题设置   |HomePageUpdateTitleParam  | HomePageUpdateTitleParam   |

**HomePageUpdateNameParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdatePostParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdateSeatParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdateTitleParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«HomePageAllDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取桌牌模板列表

**接口描述**:桌牌模板

**接口地址**:`/api/home_page/all_deaskcard`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"backgroundImg": "setting/homepage/homepage.png",
			"id": 0,
			"isSelected": false,
			"nameSetting": {
				"color": "0xfdde4e",
				"fontsize_width_percent": 0.11,
				"horizontal_center_percent": 0.5,
				"isBold": false,
				"isItalic": false,
				"isShow": true,
				"isUnderline": false,
				"title": "与会者",
				"typeface": "楷体",
				"vertical_center_percent": 0.44
			},
			"postSetting": {
				"color": "0xfdde4e",
				"fontsize_width_percent": 0.05,
				"horizontal_center_percent": 0.75,
				"isBold": false,
				"isItalic": false,
				"isShow": true,
				"isUnderline": false,
				"title": "职位",
				"typeface": "楷体",
				"vertical_center_percent": 0.75
			},
			"screenShoot": "",
			"seatSetting": {
				"color": "0xfdde4e",
				"fontsize_width_percent": 0.05,
				"horizontal_center_percent": 0.3,
				"isBold": false,
				"isItalic": false,
				"isShow": true,
				"isUnderline": false,
				"title": "座位号",
				"typeface": "楷体",
				"vertical_center_percent": 0.75
			},
			"title": "",
			"titleSetting": {
				"color": "0xfdde4e",
				"fontsize_width_percent": 0.04,
				"horizontal_center_percent": 0.5,
				"isBold": false,
				"isItalic": false,
				"isShow": true,
				"isUnderline": false,
				"title": "会议标题",
				"typeface": "楷体",
				"vertical_center_percent": 0.2
			}
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | HomePageAllDTO   |
|message|   |string  |    |



**schema属性说明**




**HomePageAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|backgroundImg | 模板背景图片   |string  |    |
|id | 首页模板id   |integer(int64)  |    |
|isSelected | 是否当前模板   |boolean  |    |
|nameSetting | 姓名设置   |HomePageUpdateNameParam  | HomePageUpdateNameParam   |
|postSetting | 职位设置   |HomePageUpdatePostParam  | HomePageUpdatePostParam   |
|screenShoot | 缩略图   |string  |    |
|seatSetting | 座位设置   |HomePageUpdateSeatParam  | HomePageUpdateSeatParam   |
|title | 首页模板标题   |string  |    |
|titleSetting | 标题设置   |HomePageUpdateTitleParam  | HomePageUpdateTitleParam   |

**HomePageUpdateNameParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdatePostParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdateSeatParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdateTitleParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«HomePageAllDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取客户端首页显示详情(会前安排)

**接口描述**:客户端

**接口地址**:`/api/home_page/client_display`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"backgroundImg": "",
		"nameSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.11,
			"horizontal_center_percent": 0.5,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "与会者",
			"typeface": "楷体",
			"vertical_center_percent": 0.44
		},
		"postSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.05,
			"horizontal_center_percent": 0.75,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "职位",
			"typeface": "楷体",
			"vertical_center_percent": 0.75
		},
		"seatSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.05,
			"horizontal_center_percent": 0.3,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "座位号",
			"typeface": "楷体",
			"vertical_center_percent": 0.75
		},
		"titleSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.04,
			"horizontal_center_percent": 0.5,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "会议标题",
			"typeface": "楷体",
			"vertical_center_percent": 0.2
		}
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |HomePageClientDisplayDTO  | HomePageClientDisplayDTO   |
|message|   |string  |    |



**schema属性说明**




**HomePageClientDisplayDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|backgroundImg | 模板背景图片   |string  |    |
|nameSetting | 姓名设置   |HomePageUpdateNameParam  | HomePageUpdateNameParam   |
|postSetting | 职位设置   |HomePageUpdatePostParam  | HomePageUpdatePostParam   |
|seatSetting | 座位设置   |HomePageUpdateSeatParam  | HomePageUpdateSeatParam   |
|titleSetting | 标题设置   |HomePageUpdateTitleParam  | HomePageUpdateTitleParam   |

**HomePageUpdateNameParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdatePostParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdateSeatParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdateTitleParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«HomePageClientDisplayDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 删除

**接口描述**:首页模板

**接口地址**:`/api/home_page/delete`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取桌牌首页显示详情(会前安排)

**接口描述**:客户端

**接口地址**:`/api/home_page/deskcard_display`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"backgroundImg": "",
		"nameSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.11,
			"horizontal_center_percent": 0.5,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "与会者",
			"typeface": "楷体",
			"vertical_center_percent": 0.44
		},
		"postSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.05,
			"horizontal_center_percent": 0.75,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "职位",
			"typeface": "楷体",
			"vertical_center_percent": 0.75
		},
		"seatSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.05,
			"horizontal_center_percent": 0.3,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "座位号",
			"typeface": "楷体",
			"vertical_center_percent": 0.75
		},
		"titleSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.04,
			"horizontal_center_percent": 0.5,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "会议标题",
			"typeface": "楷体",
			"vertical_center_percent": 0.2
		}
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |HomePageClientDisplayDTO  | HomePageClientDisplayDTO   |
|message|   |string  |    |



**schema属性说明**




**HomePageClientDisplayDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|backgroundImg | 模板背景图片   |string  |    |
|nameSetting | 姓名设置   |HomePageUpdateNameParam  | HomePageUpdateNameParam   |
|postSetting | 职位设置   |HomePageUpdatePostParam  | HomePageUpdatePostParam   |
|seatSetting | 座位设置   |HomePageUpdateSeatParam  | HomePageUpdateSeatParam   |
|titleSetting | 标题设置   |HomePageUpdateTitleParam  | HomePageUpdateTitleParam   |

**HomePageUpdateNameParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdatePostParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdateSeatParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdateTitleParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«HomePageClientDisplayDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取单个模板详情

**接口描述**:首页模板,桌牌模板

**接口地址**:`/api/home_page/findById`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"backgroundImg": "setting/homepage/homepage.png",
		"id": 0,
		"isSelected": false,
		"nameSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.11,
			"horizontal_center_percent": 0.5,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "与会者",
			"typeface": "楷体",
			"vertical_center_percent": 0.44
		},
		"postSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.05,
			"horizontal_center_percent": 0.75,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "职位",
			"typeface": "楷体",
			"vertical_center_percent": 0.75
		},
		"screenShoot": "",
		"seatSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.05,
			"horizontal_center_percent": 0.3,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "座位号",
			"typeface": "楷体",
			"vertical_center_percent": 0.75
		},
		"title": "",
		"titleSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.04,
			"horizontal_center_percent": 0.5,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "会议标题",
			"typeface": "楷体",
			"vertical_center_percent": 0.2
		}
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |HomePageAllDTO  | HomePageAllDTO   |
|message|   |string  |    |



**schema属性说明**




**HomePageAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|backgroundImg | 模板背景图片   |string  |    |
|id | 首页模板id   |integer(int64)  |    |
|isSelected | 是否当前模板   |boolean  |    |
|nameSetting | 姓名设置   |HomePageUpdateNameParam  | HomePageUpdateNameParam   |
|postSetting | 职位设置   |HomePageUpdatePostParam  | HomePageUpdatePostParam   |
|screenShoot | 缩略图   |string  |    |
|seatSetting | 座位设置   |HomePageUpdateSeatParam  | HomePageUpdateSeatParam   |
|title | 首页模板标题   |string  |    |
|titleSetting | 标题设置   |HomePageUpdateTitleParam  | HomePageUpdateTitleParam   |

**HomePageUpdateNameParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdatePostParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdateSeatParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdateTitleParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«HomePageAllDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 编辑首页模板

**接口描述**:首页模板

**接口地址**:`/api/home_page/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"backgroundImg": "setting/homepage/homepage.png",
	"id": 1,
	"nameSetting": {
		"color": "0xfdde4e",
		"fontsize_width_percent": 0.11,
		"horizontal_center_percent": 0.5,
		"isBold": false,
		"isItalic": false,
		"isShow": true,
		"isUnderline": false,
		"title": "与会者",
		"typeface": "楷体",
		"vertical_center_percent": 0.44
	},
	"postSetting": {
		"color": "0xfdde4e",
		"fontsize_width_percent": 0.05,
		"horizontal_center_percent": 0.75,
		"isBold": false,
		"isItalic": false,
		"isShow": true,
		"isUnderline": false,
		"title": "职位",
		"typeface": "楷体",
		"vertical_center_percent": 0.75
	},
	"seatSetting": {
		"color": "0xfdde4e",
		"fontsize_width_percent": 0.05,
		"horizontal_center_percent": 0.3,
		"isBold": false,
		"isItalic": false,
		"isShow": true,
		"isUnderline": false,
		"title": "座位号",
		"typeface": "楷体",
		"vertical_center_percent": 0.75
	},
	"title": "模板标题1",
	"titleSetting": {
		"color": "0xfdde4e",
		"fontsize_width_percent": 0.04,
		"horizontal_center_percent": 0.5,
		"isBold": false,
		"isItalic": false,
		"isShow": true,
		"isUnderline": false,
		"title": "会议标题",
		"typeface": "楷体",
		"vertical_center_percent": 0.2
	}
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |HomePageUpdateParam  | HomePageUpdateParam   |

**schema属性说明**



**HomePageUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|backgroundImg| 模板背景图片  | body | false |string  |    |
|id| 模板id  | body | false |integer(int64)  |    |
|nameSetting| 姓名设置  | body | false |HomePageUpdateNameParam  | HomePageUpdateNameParam   |
|postSetting| 职位设置  | body | false |HomePageUpdatePostParam  | HomePageUpdatePostParam   |
|seatSetting| 座位设置  | body | false |HomePageUpdateSeatParam  | HomePageUpdateSeatParam   |
|title| 模板标题  | body | false |string  |    |
|titleSetting| 标题设置  | body | false |HomePageUpdateTitleParam  | HomePageUpdateTitleParam   |

**HomePageUpdateNameParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|color| 颜色  | body | false |string  |    |
|fontsize_width_percent| （字体/宽度）的比例  | body | false |number(float)  |    |
|horizontal_center_percent| 文字中心相对总宽度的比例  | body | false |number(float)  |    |
|isBold| 是否加粗  | body | false |boolean  |    |
|isItalic| 是否斜体  | body | false |boolean  |    |
|isShow| 是否显示  | body | false |boolean  |    |
|isUnderline| 下划线  | body | false |boolean  |    |
|title| 显示文字  | body | false |string  |    |
|typeface| 字体  | body | false |string  |    |
|vertical_center_percent| 文字中心相对总高度的比例  | body | false |number(float)  |    |

**HomePageUpdatePostParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|color| 颜色  | body | false |string  |    |
|fontsize_width_percent| （字体/宽度）的比例  | body | false |number(float)  |    |
|horizontal_center_percent| 文字中心相对总宽度的比例  | body | false |number(float)  |    |
|isBold| 是否加粗  | body | false |boolean  |    |
|isItalic| 是否斜体  | body | false |boolean  |    |
|isShow| 是否显示  | body | false |boolean  |    |
|isUnderline| 下划线  | body | false |boolean  |    |
|title| 显示文字  | body | false |string  |    |
|typeface| 字体  | body | false |string  |    |
|vertical_center_percent| 文字中心相对总高度的比例  | body | false |number(float)  |    |

**HomePageUpdateSeatParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|color| 颜色  | body | false |string  |    |
|fontsize_width_percent| （字体/宽度）的比例  | body | false |number(float)  |    |
|horizontal_center_percent| 文字中心相对总宽度的比例  | body | false |number(float)  |    |
|isBold| 是否加粗  | body | false |boolean  |    |
|isItalic| 是否斜体  | body | false |boolean  |    |
|isShow| 是否显示  | body | false |boolean  |    |
|isUnderline| 下划线  | body | false |boolean  |    |
|title| 显示文字  | body | false |string  |    |
|typeface| 字体  | body | false |string  |    |
|vertical_center_percent| 文字中心相对总高度的比例  | body | false |number(float)  |    |

**HomePageUpdateTitleParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|color| 颜色  | body | false |string  |    |
|fontsize_width_percent| （字体/宽度）的比例  | body | false |number(float)  |    |
|horizontal_center_percent| 文字中心相对总宽度的比例  | body | false |number(float)  |    |
|isBold| 是否加粗  | body | false |boolean  |    |
|isItalic| 是否斜体  | body | false |boolean  |    |
|isShow| 是否显示  | body | false |boolean  |    |
|isUnderline| 下划线  | body | false |boolean  |    |
|title| 显示文字  | body | false |string  |    |
|typeface| 字体  | body | false |string  |    |
|vertical_center_percent| 文字中心相对总高度的比例  | body | false |number(float)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 上传首页背景

**接口描述**:首页模板,我的申请

**接口地址**:`/api/home_page/upload_img`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# index

## 测试后台可用


**接口描述**:


**接口地址**:`/api/live`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 退出登录


**接口描述**:


**接口地址**:`/api/logout`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 创建上传目录


**接口描述**:


**接口地址**:`/api/mkdir`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"path": "/home/html/uploads/20200202/"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |IndexMkdirParam  | IndexMkdirParam   |

**schema属性说明**



**IndexMkdirParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|path| 路径  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 测试


**接口描述**:


**接口地址**:`/api/test`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 测试短信


**接口描述**:


**接口地址**:`/api/test_sms`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"phone": "13221005780",
	"msg": "即将召开",
	"meetingTitle": "第一次无纸化会议",
	"time": "11:30-12:00",
	"date": "10月28日",
	"room": "成都卷烟厂二楼第一会议室"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |IndexTestSmsParam  | IndexTestSmsParam   |

**schema属性说明**



**IndexTestSmsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|phone| 手机号  | body | true |string  |    |
|msg| 状态  | body | true |string  |    |
|meetingTitle| 会议标题  | body | true |string  |    |
|time| 时间  | body | true |string  |    |
|date| 日期  | body | true |string  |    |
|room| 地点  | body | true |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 测试单发websocket


**接口描述**:


**接口地址**:`/api/test_websocket`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"fileNames": [],
	"groupId": "",
	"receiverId": "",
	"senderId": "",
	"url": "uploads/qwer.docx"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |IndexTestWebsocketParam  | IndexTestWebsocketParam   |

**schema属性说明**



**IndexTestWebsocketParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|fileNames|   | body | false |array  |    |
|groupId|   | body | false |string  |    |
|receiverId|   | body | false |string  |    |
|senderId|   | body | false |string  |    |
|url| url  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 测试政务微信


**接口描述**:


**接口地址**:`/api/test_zw_wechat`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"phone": "13221005780",
	"bigTit": "会议审核",
	"title": "您有新的审核信息",
	"weburl": "http://www.baidu.com/",
	"meeting": "成都卷烟厂第一次无纸化会议",
	"time": "11-30 10:00 - 11:30",
	"address": "成都卷烟厂二楼第一会议室"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |IndexTestZwWechatParam  | IndexTestZwWechatParam   |

**schema属性说明**



**IndexTestZwWechatParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|phone| 手机号  | body | true |string  |    |
|bigTit| 大标题  | body | true |string  |    |
|title| 小标题  | body | true |string  |    |
|weburl| 跳转页面  | body | true |string  |    |
|meeting| 会议名称  | body | true |string  |    |
|time| 时间  | body | true |string  |    |
|address| 地点  | body | true |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# issue

## 新建议题

**接口描述**:我的申请

**接口地址**:`/api/issue/add`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"issue": "议题一",
	"mid": 1,
	"proposer": "王大锤",
	"relatedUnit": "补充单位",
	"remark": "备注信息",
	"speaker": "王二锤"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |IssueAddParam  | IssueAddParam   |

**schema属性说明**



**IssueAddParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|issue| 议题名称  | body | false |string  |    |
|mid| 会议id  | body | false |integer(int64)  |    |
|proposer| 提议人  | body | false |string  |    |
|relatedUnit| 补充/涉及单位  | body | false |string  |    |
|remark| 备注  | body | false |string  |    |
|speaker| 演讲人  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## （议题审核模式）新建议题

**接口描述**:议题列表

**接口地址**:`/api/issue/add2`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"issue": "议题一",
	"remark": "材料备注"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |IssueAdd2Param  | IssueAdd2Param   |

**schema属性说明**



**IssueAdd2Param**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|issue| 议题名称  | body | false |string  |    |
|remark| 材料备注  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 删除

**接口描述**:我的申请

**接口地址**:`/api/issue/delete`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## （议题审核模式）删除

**接口描述**:议题列表

**接口地址**:`/api/issue/delete2`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"issueId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |IssueIdParam  | IssueIdParam   |

**schema属性说明**



**IssueIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|issueId| 议题id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 议题上下移

**接口描述**:我的申请

**接口地址**:`/api/issue/exchange_sort`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"firstId": 1,
	"secondId": 2
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |CommonExchangeSortParam  | CommonExchangeSortParam   |

**schema属性说明**



**CommonExchangeSortParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|firstId| 第一个主键id  | body | false |integer(int64)  |    |
|secondId| 第二个主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议议题列表

**接口描述**:我的申请,会议查询,文件归档,客户端

**接口地址**:`/api/issue/meeting`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingIdParam  | MeetingIdParam   |

**schema属性说明**



**MeetingIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"count": 0,
			"issue": "",
			"issueId": 0,
			"mid": 0,
			"proposer": "",
			"relatedUnit": "",
			"remark": "",
			"speaker": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | IssueMeetingDTO   |
|message|   |string  |    |



**schema属性说明**




**IssueMeetingDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|count |    |integer(int32)  |    |
|issue |    |string  |    |
|issueId |    |integer(int64)  |    |
|mid |    |integer(int64)  |    |
|proposer |    |string  |    |
|relatedUnit |    |string  |    |
|remark |    |string  |    |
|speaker |    |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«IssueMeetingDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## （议题审核模式）编辑会议议题

**接口描述**:议题列表

**接口地址**:`/api/issue/meeting_issues`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"issues": [],
	"mid": 0
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |IssueMeetingIssuesParam  | IssueMeetingIssuesParam   |

**schema属性说明**



**IssueMeetingIssuesParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|issues| 议题id数组  | body | false |array  |    |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## （议题审核模式）我的议题列表

**接口描述**:议题列表

**接口地址**:`/api/issue/my_issues`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"isReviewPassedOnly": false,
	"isSelf": false
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |IssueMyIssuesParam  | IssueMyIssuesParam   |

**schema属性说明**



**IssueMyIssuesParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|isReviewPassedOnly| 是否只看已审核通过  | body | false |boolean  |    |
|isSelf| 是否自己创建的议题  | body | false |boolean  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"createUid": 0,
			"issue": "",
			"issueId": 0,
			"remark": "",
			"sort": 0
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | IssueMyIssuesDTO   |
|message|   |string  |    |



**schema属性说明**




**IssueMyIssuesDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|createUid |    |integer(int64)  |    |
|issue |    |string  |    |
|issueId |    |integer(int64)  |    |
|remark |    |string  |    |
|sort |    |integer(int32)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«IssueMyIssuesDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 编辑议题

**接口描述**:我的申请

**接口地址**:`/api/issue/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"issue": "议题一",
	"issueId": 1,
	"proposer": "王大锤",
	"relatedUnit": "补充单位",
	"remark": "备注信息",
	"speaker": "王二锤"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |IssueUpdateParam  | IssueUpdateParam   |

**schema属性说明**



**IssueUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|issue| 议题名称  | body | false |string  |    |
|issueId| 议题id  | body | false |integer(int64)  |    |
|proposer| 提议人  | body | false |string  |    |
|relatedUnit| 补充/涉及单位  | body | false |string  |    |
|remark| 备注  | body | false |string  |    |
|speaker| 演讲人  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## （议题审核模式）编辑议题

**接口描述**:议题列表

**接口地址**:`/api/issue/update2`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"issue": "议题一",
	"issueId": 1,
	"remark": "备注信息"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |IssueUpdate2Param  | IssueUpdate2Param   |

**schema属性说明**



**IssueUpdate2Param**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|issue| 议题名称  | body | false |string  |    |
|issueId| 议题id  | body | false |integer(int64)  |    |
|remark| 备注  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 超大文件上传码

**接口描述**:我的申请

**接口地址**:`/api/issue/upload_code`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"issueId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |IssueUploadCodeParam  | IssueUploadCodeParam   |

**schema属性说明**



**IssueUploadCodeParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|issueId| 议题id  | body | true |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# mu_file

## 上传文件


**接口描述**:


**接口地址**:`/api/mu_file/add/{muid}`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |
|muid| muid  | path | true |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 保存会议材料到我的文件，并返回我的文件进行批注


**接口描述**:


**接口地址**:`/api/mu_file/comment_new_file/{fileId}/muid/{muid}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|fileId| fileId  | path | true |integer  |    |
|muid| muid  | path | true |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"commentPdf": "",
		"fileId": 0,
		"name": ""
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |MufileCommentNewFileDTO  | MufileCommentNewFileDTO   |
|message|   |string  |    |



**schema属性说明**




**MufileCommentNewFileDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|commentPdf | 批注用pdf   |string  |    |
|fileId | 文件id   |integer(int64)  |    |
|name | 文件名称   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«MufileCommentNewFileDTO»|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 删除


**接口描述**:


**接口地址**:`/api/mu_file/delete`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取编辑地址并保存到我的文件

**接口描述**:客户端

**接口地址**:`/api/mu_file/edit_new_file/{fileId}/muid/{muid}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|fileId| fileId  | path | true |integer  |    |
|muid| muid  | path | true |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 我的文件


**接口描述**:


**接口地址**:`/api/mu_file/list/{muid}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|muid| muid  | path | true |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"commentPdf": "",
			"edit": "",
			"ext": "",
			"gmtCreate": "",
			"id": 0,
			"name": "",
			"size": "",
			"uploadUser": "",
			"url": "",
			"view": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | FileListDTO   |
|message|   |string  |    |



**schema属性说明**




**FileListDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|commentPdf | 批注用pdf   |string  |    |
|edit | 编辑url   |string  |    |
|ext | 扩展名   |string  |    |
|gmtCreate | 上传时间   |string(date-time)  |    |
|id | 文件id   |integer(int64)  |    |
|name | 文件名称   |string  |    |
|size | 文件大小   |string  |    |
|uploadUser | 上传人   |string  |    |
|url | 文件url   |string  |    |
|view | 查看url   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«FileListDTO»»|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# meeting_picture

## 新建截图

**接口描述**:客户端

**接口地址**:`/api/meeting_picture/add`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"fileId": 0,
	"fileName": "",
	"mid": 0,
	"muid": 0,
	"url": ""
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingPictureAddParam  | MeetingPictureAddParam   |

**schema属性说明**



**MeetingPictureAddParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|fileId| 文件id，白板为0  | body | false |integer(int64)  |    |
|fileName| 文件名称，白板为空字符串  | body | false |string  |    |
|mid| 会议id  | body | false |integer(int64)  |    |
|muid| 会议人员id  | body | false |integer(int64)  |    |
|url| 文件存储路径  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取本人材料的截图

**接口描述**:客户端

**接口地址**:`/api/meeting_picture/file`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"fileId": 1,
	"muid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingPictureFileParam  | MeetingPictureFileParam   |

**schema属性说明**



**MeetingPictureFileParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|fileId| 文件id  | body | false |integer(int64)  |    |
|muid| 会议人员id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"fileName": "",
		"pictures": []
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |MeetingPictureDTO  | MeetingPictureDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingPictureDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|fileName | 文件名称   |string  |    |
|pictures | 图片集   |array  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«MeetingPictureDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取会议所有的截图

**接口描述**:会议查询,客户端

**接口地址**:`/api/meeting_picture/get_by_mid`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingIdParam  | MeetingIdParam   |

**schema属性说明**



**MeetingIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"fileName": "",
			"pictures": []
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MeetingPictureDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingPictureDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|fileName | 文件名称   |string  |    |
|pictures | 图片集   |array  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MeetingPictureDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取muid的截图

**接口描述**:客户端

**接口地址**:`/api/meeting_picture/get_by_muid`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"muid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MUIDParam  | MUIDParam   |

**schema属性说明**



**MUIDParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|muid| 会议人员id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"fileName": "",
			"pictures": []
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MeetingPictureDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingPictureDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|fileName | 文件名称   |string  |    |
|pictures | 图片集   |array  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MeetingPictureDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 上传截图


**接口描述**:


**接口地址**:`/api/meeting_picture/upload`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# meeting_type

## 新增会议分类

**接口描述**:会议分类

**接口地址**:`/api/meeting_type/add`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"name": "党委会议"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingTypeAddParam  | MeetingTypeAddParam   |

**schema属性说明**



**MeetingTypeAddParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|name| 分类名称  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 查询所有分类


**接口描述**:


**接口地址**:`/api/meeting_type/all`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"meetingTypeId": 0,
			"name": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MeetingTypeAllDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingTypeAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|meetingTypeId | 分类id   |integer(int64)  |    |
|name | 分类名称   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MeetingTypeAllDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 删除

**接口描述**:会议分类

**接口地址**:`/api/meeting_type/delete`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"meetingTypeId": 0
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingTypeIdParam  | MeetingTypeIdParam   |

**schema属性说明**



**MeetingTypeIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|meetingTypeId| 分类id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 查询会议分类选中情况


**接口描述**:


**接口地址**:`/api/meeting_type/meeting`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingIdParam  | MeetingIdParam   |

**schema属性说明**



**MeetingIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"isSelected": false,
			"meetingTypeId": 0,
			"name": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MeetingTypeMeetingDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingTypeMeetingDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|isSelected | 是否选中   |boolean  |    |
|meetingTypeId | 分类id   |integer(int64)  |    |
|name | 分类名称   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MeetingTypeMeetingDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 设置会议分类选中情况


**接口描述**:


**接口地址**:`/api/meeting_type/meeting_update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"meetingTypeId": 0,
	"mid": 0
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingTypeMeetingUpdateParam  | MeetingTypeMeetingUpdateParam   |

**schema属性说明**



**MeetingTypeMeetingUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|meetingTypeId| 分类id  | body | false |integer(int64)  |    |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 编辑

**接口描述**:会议分类

**接口地址**:`/api/meeting_type/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"meetingTypeId": 1,
	"name": "党委会议"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingTypeUpdateParam  | MeetingTypeUpdateParam   |

**schema属性说明**



**MeetingTypeUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|meetingTypeId| 分类id  | body | false |integer(int64)  |    |
|name| 分类名称  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# meeting

## 新建会议（待提交）

**接口描述**:我的申请

**接口地址**:`/api/meeting/add`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 取消会议

**接口描述**:我的申请,客户端

**接口地址**:`/api/meeting/cancel`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 取消申请

**接口描述**:我的申请

**接口地址**:`/api/meeting/cancel_push`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 关闭会议

**接口描述**:我的申请,客户端

**接口地址**:`/api/meeting/close`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingIdParam  | MeetingIdParam   |

**schema属性说明**



**MeetingIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 复制会议

**接口描述**:我的申请

**接口地址**:`/api/meeting/copy`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingIdParam  | MeetingIdParam   |

**schema属性说明**



**MeetingIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 删除

**接口描述**:我的申请

**接口地址**:`/api/meeting/delete`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingIdParam  | MeetingIdParam   |

**schema属性说明**



**MeetingIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取桌牌首页设置

**接口描述**:我的申请

**接口地址**:`/api/meeting/deskcard`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"backgroundImg": "setting/homepage/homepage.png",
		"nameSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.11,
			"horizontal_center_percent": 0.5,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "与会者",
			"typeface": "楷体",
			"vertical_center_percent": 0.44
		},
		"postSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.05,
			"horizontal_center_percent": 0.75,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "职位",
			"typeface": "楷体",
			"vertical_center_percent": 0.75
		},
		"seatSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.05,
			"horizontal_center_percent": 0.3,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "座位号",
			"typeface": "楷体",
			"vertical_center_percent": 0.75
		},
		"titleSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.04,
			"horizontal_center_percent": 0.5,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "会议标题",
			"typeface": "楷体",
			"vertical_center_percent": 0.2
		}
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |MeetingHomePageDTO  | MeetingHomePageDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingHomePageDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|backgroundImg | 模板背景图片   |string  |    |
|nameSetting | 姓名设置   |HomePageUpdateNameParam  | HomePageUpdateNameParam   |
|postSetting | 职位设置   |HomePageUpdatePostParam  | HomePageUpdatePostParam   |
|seatSetting | 座位设置   |HomePageUpdateSeatParam  | HomePageUpdateSeatParam   |
|titleSetting | 标题设置   |HomePageUpdateTitleParam  | HomePageUpdateTitleParam   |

**HomePageUpdateNameParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdatePostParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdateSeatParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdateTitleParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«MeetingHomePageDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取会议详情

**接口描述**:我的申请

**接口地址**:`/api/meeting/detail`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"assist": "",
		"chairman": "",
		"endTime": "",
		"hostPassword": "",
		"id": 0,
		"introduce": "",
		"password": "",
		"remark": "",
		"rooms": [
			{
				"room": "",
				"roomId": 0
			}
		],
		"startTime": "",
		"status": "",
		"statusEnum": "",
		"title": "",
		"topic": "",
		"type": 0
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |MeetingDetailDTO  | MeetingDetailDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingDetailDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|assist | 秘书   |string  |    |
|chairman | 主席   |string  |    |
|endTime | 结束时间   |string(date-time)  |    |
|hostPassword | 主持人密码   |string  |    |
|id | 会议id   |integer(int64)  |    |
|introduce | 会议简介   |string  |    |
|password | 会议密码   |string  |    |
|remark | 备注   |string  |    |
|rooms | 选中的会议室（可多个）   |array  | MeetingDetailRoomDTO   |
|startTime | 开始时间   |string(date-time)  |    |
|status | 会议状态说明   |string  |    |
|statusEnum | 会议状态,可用值:UNSUBMITTED,DELETED,APPLYING,ENABLED,ENDED,REFUSED,CANCELED,CLOSED   |string  |    |
|title | 会议名称   |string  |    |
|topic | 主题   |string  |    |
|type | 会议类型,1、会前安排，2、帐密登录，3、会议密码   |integer(int32)  |    |

**MeetingDetailRoomDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|room | 会议室   |string  |    |
|roomId | 会议室id   |integer(int64)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«MeetingDetailDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 挂起会议

**接口描述**:我的申请

**接口地址**:`/api/meeting/end`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingIdParam  | MeetingIdParam   |

**schema属性说明**



**MeetingIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取会议首页设置

**接口描述**:我的申请

**接口地址**:`/api/meeting/home_page`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"backgroundImg": "setting/homepage/homepage.png",
		"nameSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.11,
			"horizontal_center_percent": 0.5,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "与会者",
			"typeface": "楷体",
			"vertical_center_percent": 0.44
		},
		"postSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.05,
			"horizontal_center_percent": 0.75,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "职位",
			"typeface": "楷体",
			"vertical_center_percent": 0.75
		},
		"seatSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.05,
			"horizontal_center_percent": 0.3,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "座位号",
			"typeface": "楷体",
			"vertical_center_percent": 0.75
		},
		"titleSetting": {
			"color": "0xfdde4e",
			"fontsize_width_percent": 0.04,
			"horizontal_center_percent": 0.5,
			"isBold": false,
			"isItalic": false,
			"isShow": true,
			"isUnderline": false,
			"title": "会议标题",
			"typeface": "楷体",
			"vertical_center_percent": 0.2
		}
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |MeetingHomePageDTO  | MeetingHomePageDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingHomePageDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|backgroundImg | 模板背景图片   |string  |    |
|nameSetting | 姓名设置   |HomePageUpdateNameParam  | HomePageUpdateNameParam   |
|postSetting | 职位设置   |HomePageUpdatePostParam  | HomePageUpdatePostParam   |
|seatSetting | 座位设置   |HomePageUpdateSeatParam  | HomePageUpdateSeatParam   |
|titleSetting | 标题设置   |HomePageUpdateTitleParam  | HomePageUpdateTitleParam   |

**HomePageUpdateNameParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdatePostParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdateSeatParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**HomePageUpdateTitleParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|color | 颜色   |string  |    |
|fontsize_width_percent | （字体/宽度）的比例   |number(float)  |    |
|horizontal_center_percent | 文字中心相对总宽度的比例   |number(float)  |    |
|isBold | 是否加粗   |boolean  |    |
|isItalic | 是否斜体   |boolean  |    |
|isShow | 是否显示   |boolean  |    |
|isUnderline | 下划线   |boolean  |    |
|title | 显示文字   |string  |    |
|typeface | 字体   |string  |    |
|vertical_center_percent | 文字中心相对总高度的比例   |number(float)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«MeetingHomePageDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议列表

**接口描述**:会议列表,会议管理

**接口地址**:`/api/meeting/list`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"pageNum": 1,
	"pageSize": 10
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingListParam  | MeetingListParam   |

**schema属性说明**



**MeetingListParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|pageNum| 当前页号  | body | false |integer(int32)  |    |
|pageSize| 每页记录数  | body | false |integer(int32)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"hasNext": true,
		"list": [
			{
				"id": 0,
				"title": "",
				"time": "",
				"room": "",
				"applicant": "",
				"statusStr": "",
				"status": ""
			}
		],
		"pageNum": 0,
		"pageSize": 0,
		"totalCount": 0,
		"totalPage": 0
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |PageDTO«MeetingListDTO»  | PageDTO«MeetingListDTO»   |
|message|   |string  |    |



**schema属性说明**




**PageDTO«MeetingListDTO»**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|hasNext |    |boolean  |    |
|list |    |array  | MeetingListDTO   |
|pageNum |    |integer(int32)  |    |
|pageSize |    |integer(int32)  |    |
|totalCount |    |integer(int32)  |    |
|totalPage |    |integer(int32)  |    |

**MeetingListDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|id | 会议id   |integer(int64)  |    |
|title | 会议名称   |string  |    |
|time | 会议时间   |string  |    |
|room | 会议地点   |string  |    |
|applicant | 申请人   |string  |    |
|statusStr | 会议状态字符串   |string  |    |
|status | 会议状态:进行中,已挂起,可用值:UNSUBMITTED,DELETED,APPLYING,ENABLED,ENDED,REFUSED,CANCELED,CLOSED   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«PageDTO«MeetingListDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 我的申请

**接口描述**:我的申请

**接口地址**:`/api/meeting/my_applys`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"pageNum": 1,
	"pageSize": 10,
	"status": "UNSUBMITTED"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingMyApplysParam  | MeetingMyApplysParam   |

**schema属性说明**



**MeetingMyApplysParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|pageNum| 当前页号  | body | false |integer(int32)  |    |
|pageSize| 每页记录数  | body | false |integer(int32)  |    |
|status| 状态,可用值:UNSUBMITTED,DELETED,APPLYING,ENABLED,ENDED,REFUSED,CANCELED,CLOSED  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"hasNext": true,
		"list": [
			{
				"id": 0,
				"title": "",
				"time": "",
				"room": "",
				"statusStr": "",
				"status": ""
			}
		],
		"pageNum": 0,
		"pageSize": 0,
		"totalCount": 0,
		"totalPage": 0
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |PageDTO«MeetingApplysDTO»  | PageDTO«MeetingApplysDTO»   |
|message|   |string  |    |



**schema属性说明**




**PageDTO«MeetingApplysDTO»**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|hasNext |    |boolean  |    |
|list |    |array  | MeetingApplysDTO   |
|pageNum |    |integer(int32)  |    |
|pageSize |    |integer(int32)  |    |
|totalCount |    |integer(int32)  |    |
|totalPage |    |integer(int32)  |    |

**MeetingApplysDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|id | 会议id   |integer(int64)  |    |
|title | 会议名称   |string  |    |
|time | 会议时间   |string  |    |
|room | 会议地点   |string  |    |
|statusStr | 会议状态字符串   |string  |    |
|status | 会议状态:待提交,申请中,进行中,已结束,已拒绝,已取消,可用值:UNSUBMITTED,DELETED,APPLYING,ENABLED,ENDED,REFUSED,CANCELED,CLOSED   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«PageDTO«MeetingApplysDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 我的会议

**接口描述**:我的会议

**接口地址**:`/api/meeting/my_meetings`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"id": 0,
			"title": "",
			"time": "",
			"room": "",
			"statusStr": "",
			"status": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MeetingApplysDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingApplysDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|id | 会议id   |integer(int64)  |    |
|title | 会议名称   |string  |    |
|time | 会议时间   |string  |    |
|room | 会议地点   |string  |    |
|statusStr | 会议状态字符串   |string  |    |
|status | 会议状态:待提交,申请中,进行中,已结束,已拒绝,已取消,可用值:UNSUBMITTED,DELETED,APPLYING,ENABLED,ENDED,REFUSED,CANCELED,CLOSED   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MeetingApplysDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 提交申请（无审核环节->进行中）

**接口描述**:我的申请

**接口地址**:`/api/meeting/push`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1,
	"reviewUid": 2
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingPushParam  | MeetingPushParam   |

**schema属性说明**



**MeetingPushParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |
|reviewUid| 审核人员id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 重置会议包含所有会议室的当前会议（客户端主持人结束会议）


**接口描述**:


**接口地址**:`/api/meeting/reset_meeting_rooms_current_mid`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingIdParam  | MeetingIdParam   |

**schema属性说明**



**MeetingIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议室门牌当前和下场会议


**接口描述**:


**接口地址**:`/api/meeting/roomcard_meetings`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"pageNum": 1,
	"pageSize": 10,
	"roomId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingNologinRoomMeetingsParam  | MeetingNologinRoomMeetingsParam   |

**schema属性说明**



**MeetingNologinRoomMeetingsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|pageNum| 当前页号  | body | false |integer(int32)  |    |
|pageSize| 每页记录数  | body | false |integer(int32)  |    |
|roomId| 会议室id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"applyUser": "",
			"color": "",
			"id": 0,
			"isSelected": false,
			"room": "",
			"status": "",
			"time": "",
			"title": "",
			"topic": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MeetingRoomcardMeetingDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingRoomcardMeetingDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|applyUser | 申请人   |string  |    |
|color | 灯带颜色码   |string  |    |
|id | 会议id   |integer(int64)  |    |
|isSelected | 是否当前会议   |boolean  |    |
|room | 会议地点   |string  |    |
|status | 会议状态   |string  |    |
|time | 会议时间   |string  |    |
|title | 会议名称   |string  |    |
|topic | 会议主题   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MeetingRoomcardMeetingDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议可用会议室

**接口描述**:我的申请

**接口地址**:`/api/meeting/rooms`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"endTime": "2019-11-23 12:00",
	"mid": 1,
	"startTime": "2019-11-23 09:00"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingRoomsParam  | MeetingRoomsParam   |

**schema属性说明**



**MeetingRoomsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|endTime| 结束时间  | body | false |string(date-time)  |    |
|mid| 会议id  | body | false |integer(int64)  |    |
|startTime| 开始时间  | body | false |string(date-time)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"no": 0,
			"rooms": [
				{
					"classId": 0,
					"id": 0,
					"isUsed": false,
					"seatNum": 0,
					"title": ""
				}
			],
			"title": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MeetingRoomsDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingRoomsDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|no | 分区序号   |integer(int32)  |    |
|rooms | 分区会议室   |array  | MeetingRooms_roomDTO   |
|title | 分区名称   |string  |    |

**MeetingRooms_roomDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|classId | 会议室分区id   |integer(int64)  |    |
|id | 会议室id   |integer(int64)  |    |
|isUsed | 是否使用中   |boolean  |    |
|seatNum | 座位数   |integer(int32)  |    |
|title | 会议室名称   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MeetingRoomsDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 查询会议

**接口描述**:会议查询,客户端

**接口地址**:`/api/meeting/select`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"departmentId": 1,
	"endTime": "2020-11-23 12:00",
	"meetingName": "第一次",
	"pageNum": 1,
	"pageSize": 10,
	"roomId": 1,
	"startTime": "2017-11-23 09:00",
	"statusEnum": "ENABLED"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingSelectParam  | MeetingSelectParam   |

**schema属性说明**



**MeetingSelectParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|departmentId| 部门id  | body | false |integer(int64)  |    |
|endTime| 结束时间  | body | false |string(date-time)  |    |
|meetingName| 会议名称  | body | false |string  |    |
|pageNum| 当前页号  | body | false |integer(int32)  |    |
|pageSize| 每页记录数  | body | false |integer(int32)  |    |
|roomId| 会议室id  | body | false |integer(int64)  |    |
|startTime| 开始时间  | body | false |string(date-time)  |    |
|statusEnum| 会议状态,可用值:UNSUBMITTED,DELETED,APPLYING,ENABLED,ENDED,REFUSED,CANCELED,CLOSED  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"hasNext": true,
		"list": [
			{
				"id": 0,
				"title": "",
				"time": "",
				"room": "",
				"applicant": "",
				"remark": "",
				"statusStr": "",
				"status": ""
			}
		],
		"pageNum": 0,
		"pageSize": 0,
		"totalCount": 0,
		"totalPage": 0
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |PageDTO«MeetingSelectDTO»  | PageDTO«MeetingSelectDTO»   |
|message|   |string  |    |



**schema属性说明**




**PageDTO«MeetingSelectDTO»**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|hasNext |    |boolean  |    |
|list |    |array  | MeetingSelectDTO   |
|pageNum |    |integer(int32)  |    |
|pageSize |    |integer(int32)  |    |
|totalCount |    |integer(int32)  |    |
|totalPage |    |integer(int32)  |    |

**MeetingSelectDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|id | 会议id   |integer(int64)  |    |
|title | 会议名称   |string  |    |
|time | 会议时间   |string  |    |
|room | 会议地点   |string  |    |
|applicant | 申请人   |string  |    |
|remark | 备注/(四川中烟为人数)   |string  |    |
|statusStr | 会议状态字符串   |string  |    |
|status | 会议状态,可用值:UNSUBMITTED,DELETED,APPLYING,ENABLED,ENDED,REFUSED,CANCELED,CLOSED   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«PageDTO«MeetingSelectDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取实时签到情况


**接口描述**:


**接口地址**:`/api/meeting/signNumbers`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|mid| mid  | query | false |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"leftNo": 0,
		"signedNo": 0,
		"sumNo": 0
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |MeetingSignNumbersDTO  | MeetingSignNumbersDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingSignNumbersDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|leftNo | 未到   |integer(int32)  |    |
|signedNo | 实到   |integer(int32)  |    |
|sumNo | 应到   |integer(int32)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«MeetingSignNumbersDTO»|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 修改会议标题

**接口描述**:我的申请

**接口地址**:`/api/meeting/tit`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1,
	"title": "会议名称"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingTitParam  | MeetingTitParam   |

**schema属性说明**



**MeetingTitParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |
|title| 会议名称  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议室会议列表(会前安排模式)


**接口描述**:


**接口地址**:`/api/meeting/today_room_nologin_meetings`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"pageNum": 1,
	"pageSize": 10,
	"roomId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingNologinRoomMeetingsParam  | MeetingNologinRoomMeetingsParam   |

**schema属性说明**



**MeetingNologinRoomMeetingsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|pageNum| 当前页号  | body | false |integer(int32)  |    |
|pageSize| 每页记录数  | body | false |integer(int32)  |    |
|roomId| 会议室id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"applyUser": "",
			"id": 0,
			"isSelected": false,
			"isTodayMeeting": false,
			"room": "",
			"time": "",
			"title": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MeetingNologinRoomMeetingsDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingNologinRoomMeetingsDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|applyUser | 申请人   |string  |    |
|id | 会议id   |integer(int64)  |    |
|isSelected | 是否当前会议   |boolean  |    |
|isTodayMeeting | 是否当前会议   |boolean  |    |
|room | 会议地点   |string  |    |
|time | 会议时间   |string  |    |
|title | 会议名称   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MeetingNologinRoomMeetingsDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 重置为待提交（已拒绝、已取消可转为待提交重新申请）

**接口描述**:我的申请

**接口地址**:`/api/meeting/unsubmitted`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 更新会议

**接口描述**:我的申请

**接口地址**:`/api/meeting/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"assist": "李志霞",
	"chairman": "孙鑫",
	"endTime": "2019-11-23 12:00",
	"hostPassword": "123",
	"id": 1,
	"introduce": "会议简介内容",
	"password": "123",
	"remark": "准备30瓶水",
	"roomIds": [],
	"startTime": "2019-11-23 09:00",
	"title": "第一次员工大会",
	"topic": "技术研讨"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUpdateParam  | MeetingUpdateParam   |

**schema属性说明**



**MeetingUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|assist| 秘书  | body | false |string  |    |
|chairman| 主席  | body | false |string  |    |
|endTime| 结束时间  | body | false |string(date-time)  |    |
|hostPassword| 主持人密码  | body | false |string  |    |
|id| 会议id  | body | false |integer(int64)  |    |
|introduce| 会议简介  | body | false |string  |    |
|password| 会议密码  | body | false |string  |    |
|remark| 备注  | body | false |string  |    |
|roomIds| 会议室id(数组)  | body | false |array  |    |
|startTime| 开始时间  | body | false |string(date-time)  |    |
|title| 会议名称  | body | false |string  |    |
|topic| 主题  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 编辑桌牌首页设置

**接口描述**:我的申请

**接口地址**:`/api/meeting/update_deskcard`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"backgroundImg": "setting/homepage/homepage.png",
	"mid": 1,
	"nameSetting": {
		"color": "0xfdde4e",
		"fontsize_width_percent": 0.11,
		"horizontal_center_percent": 0.5,
		"isBold": false,
		"isItalic": false,
		"isShow": true,
		"isUnderline": false,
		"title": "与会者",
		"typeface": "楷体",
		"vertical_center_percent": 0.44
	},
	"postSetting": {
		"color": "0xfdde4e",
		"fontsize_width_percent": 0.05,
		"horizontal_center_percent": 0.75,
		"isBold": false,
		"isItalic": false,
		"isShow": true,
		"isUnderline": false,
		"title": "职位",
		"typeface": "楷体",
		"vertical_center_percent": 0.75
	},
	"seatSetting": {
		"color": "0xfdde4e",
		"fontsize_width_percent": 0.05,
		"horizontal_center_percent": 0.3,
		"isBold": false,
		"isItalic": false,
		"isShow": true,
		"isUnderline": false,
		"title": "座位号",
		"typeface": "楷体",
		"vertical_center_percent": 0.75
	},
	"titleSetting": {
		"color": "0xfdde4e",
		"fontsize_width_percent": 0.04,
		"horizontal_center_percent": 0.5,
		"isBold": false,
		"isItalic": false,
		"isShow": true,
		"isUnderline": false,
		"title": "会议标题",
		"typeface": "楷体",
		"vertical_center_percent": 0.2
	}
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUpdateHomePageParam  | MeetingUpdateHomePageParam   |

**schema属性说明**



**MeetingUpdateHomePageParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|backgroundImg| 模板背景图片  | body | false |string  |    |
|mid| 会议id  | body | false |integer(int64)  |    |
|nameSetting| 姓名设置  | body | false |HomePageUpdateNameParam  | HomePageUpdateNameParam   |
|postSetting| 职位设置  | body | false |HomePageUpdatePostParam  | HomePageUpdatePostParam   |
|seatSetting| 座位设置  | body | false |HomePageUpdateSeatParam  | HomePageUpdateSeatParam   |
|titleSetting| 标题设置  | body | false |HomePageUpdateTitleParam  | HomePageUpdateTitleParam   |

**HomePageUpdateNameParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|color| 颜色  | body | false |string  |    |
|fontsize_width_percent| （字体/宽度）的比例  | body | false |number(float)  |    |
|horizontal_center_percent| 文字中心相对总宽度的比例  | body | false |number(float)  |    |
|isBold| 是否加粗  | body | false |boolean  |    |
|isItalic| 是否斜体  | body | false |boolean  |    |
|isShow| 是否显示  | body | false |boolean  |    |
|isUnderline| 下划线  | body | false |boolean  |    |
|title| 显示文字  | body | false |string  |    |
|typeface| 字体  | body | false |string  |    |
|vertical_center_percent| 文字中心相对总高度的比例  | body | false |number(float)  |    |

**HomePageUpdatePostParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|color| 颜色  | body | false |string  |    |
|fontsize_width_percent| （字体/宽度）的比例  | body | false |number(float)  |    |
|horizontal_center_percent| 文字中心相对总宽度的比例  | body | false |number(float)  |    |
|isBold| 是否加粗  | body | false |boolean  |    |
|isItalic| 是否斜体  | body | false |boolean  |    |
|isShow| 是否显示  | body | false |boolean  |    |
|isUnderline| 下划线  | body | false |boolean  |    |
|title| 显示文字  | body | false |string  |    |
|typeface| 字体  | body | false |string  |    |
|vertical_center_percent| 文字中心相对总高度的比例  | body | false |number(float)  |    |

**HomePageUpdateSeatParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|color| 颜色  | body | false |string  |    |
|fontsize_width_percent| （字体/宽度）的比例  | body | false |number(float)  |    |
|horizontal_center_percent| 文字中心相对总宽度的比例  | body | false |number(float)  |    |
|isBold| 是否加粗  | body | false |boolean  |    |
|isItalic| 是否斜体  | body | false |boolean  |    |
|isShow| 是否显示  | body | false |boolean  |    |
|isUnderline| 下划线  | body | false |boolean  |    |
|title| 显示文字  | body | false |string  |    |
|typeface| 字体  | body | false |string  |    |
|vertical_center_percent| 文字中心相对总高度的比例  | body | false |number(float)  |    |

**HomePageUpdateTitleParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|color| 颜色  | body | false |string  |    |
|fontsize_width_percent| （字体/宽度）的比例  | body | false |number(float)  |    |
|horizontal_center_percent| 文字中心相对总宽度的比例  | body | false |number(float)  |    |
|isBold| 是否加粗  | body | false |boolean  |    |
|isItalic| 是否斜体  | body | false |boolean  |    |
|isShow| 是否显示  | body | false |boolean  |    |
|isUnderline| 下划线  | body | false |boolean  |    |
|title| 显示文字  | body | false |string  |    |
|typeface| 字体  | body | false |string  |    |
|vertical_center_percent| 文字中心相对总高度的比例  | body | false |number(float)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 编辑会议首页设置

**接口描述**:我的申请

**接口地址**:`/api/meeting/update_home_page`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"backgroundImg": "setting/homepage/homepage.png",
	"mid": 1,
	"nameSetting": {
		"color": "0xfdde4e",
		"fontsize_width_percent": 0.11,
		"horizontal_center_percent": 0.5,
		"isBold": false,
		"isItalic": false,
		"isShow": true,
		"isUnderline": false,
		"title": "与会者",
		"typeface": "楷体",
		"vertical_center_percent": 0.44
	},
	"postSetting": {
		"color": "0xfdde4e",
		"fontsize_width_percent": 0.05,
		"horizontal_center_percent": 0.75,
		"isBold": false,
		"isItalic": false,
		"isShow": true,
		"isUnderline": false,
		"title": "职位",
		"typeface": "楷体",
		"vertical_center_percent": 0.75
	},
	"seatSetting": {
		"color": "0xfdde4e",
		"fontsize_width_percent": 0.05,
		"horizontal_center_percent": 0.3,
		"isBold": false,
		"isItalic": false,
		"isShow": true,
		"isUnderline": false,
		"title": "座位号",
		"typeface": "楷体",
		"vertical_center_percent": 0.75
	},
	"titleSetting": {
		"color": "0xfdde4e",
		"fontsize_width_percent": 0.04,
		"horizontal_center_percent": 0.5,
		"isBold": false,
		"isItalic": false,
		"isShow": true,
		"isUnderline": false,
		"title": "会议标题",
		"typeface": "楷体",
		"vertical_center_percent": 0.2
	}
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUpdateHomePageParam  | MeetingUpdateHomePageParam   |

**schema属性说明**



**MeetingUpdateHomePageParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|backgroundImg| 模板背景图片  | body | false |string  |    |
|mid| 会议id  | body | false |integer(int64)  |    |
|nameSetting| 姓名设置  | body | false |HomePageUpdateNameParam  | HomePageUpdateNameParam   |
|postSetting| 职位设置  | body | false |HomePageUpdatePostParam  | HomePageUpdatePostParam   |
|seatSetting| 座位设置  | body | false |HomePageUpdateSeatParam  | HomePageUpdateSeatParam   |
|titleSetting| 标题设置  | body | false |HomePageUpdateTitleParam  | HomePageUpdateTitleParam   |

**HomePageUpdateNameParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|color| 颜色  | body | false |string  |    |
|fontsize_width_percent| （字体/宽度）的比例  | body | false |number(float)  |    |
|horizontal_center_percent| 文字中心相对总宽度的比例  | body | false |number(float)  |    |
|isBold| 是否加粗  | body | false |boolean  |    |
|isItalic| 是否斜体  | body | false |boolean  |    |
|isShow| 是否显示  | body | false |boolean  |    |
|isUnderline| 下划线  | body | false |boolean  |    |
|title| 显示文字  | body | false |string  |    |
|typeface| 字体  | body | false |string  |    |
|vertical_center_percent| 文字中心相对总高度的比例  | body | false |number(float)  |    |

**HomePageUpdatePostParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|color| 颜色  | body | false |string  |    |
|fontsize_width_percent| （字体/宽度）的比例  | body | false |number(float)  |    |
|horizontal_center_percent| 文字中心相对总宽度的比例  | body | false |number(float)  |    |
|isBold| 是否加粗  | body | false |boolean  |    |
|isItalic| 是否斜体  | body | false |boolean  |    |
|isShow| 是否显示  | body | false |boolean  |    |
|isUnderline| 下划线  | body | false |boolean  |    |
|title| 显示文字  | body | false |string  |    |
|typeface| 字体  | body | false |string  |    |
|vertical_center_percent| 文字中心相对总高度的比例  | body | false |number(float)  |    |

**HomePageUpdateSeatParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|color| 颜色  | body | false |string  |    |
|fontsize_width_percent| （字体/宽度）的比例  | body | false |number(float)  |    |
|horizontal_center_percent| 文字中心相对总宽度的比例  | body | false |number(float)  |    |
|isBold| 是否加粗  | body | false |boolean  |    |
|isItalic| 是否斜体  | body | false |boolean  |    |
|isShow| 是否显示  | body | false |boolean  |    |
|isUnderline| 下划线  | body | false |boolean  |    |
|title| 显示文字  | body | false |string  |    |
|typeface| 字体  | body | false |string  |    |
|vertical_center_percent| 文字中心相对总高度的比例  | body | false |number(float)  |    |

**HomePageUpdateTitleParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|color| 颜色  | body | false |string  |    |
|fontsize_width_percent| （字体/宽度）的比例  | body | false |number(float)  |    |
|horizontal_center_percent| 文字中心相对总宽度的比例  | body | false |number(float)  |    |
|isBold| 是否加粗  | body | false |boolean  |    |
|isItalic| 是否斜体  | body | false |boolean  |    |
|isShow| 是否显示  | body | false |boolean  |    |
|isUnderline| 下划线  | body | false |boolean  |    |
|title| 显示文字  | body | false |string  |    |
|typeface| 字体  | body | false |string  |    |
|vertical_center_percent| 文字中心相对总高度的比例  | body | false |number(float)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# meeting_user

## 来宾录入（会前安排）


**接口描述**:


**接口地址**:`/api/meeting_user/add_guest`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"company": "开发部",
	"department": "开发部",
	"deviceCode": "device_code",
	"deviceType": "",
	"name": "超级管理员",
	"post": "普通员工"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserAddGuestParam  | MeetingUserAddGuestParam   |

**schema属性说明**



**MeetingUserAddGuestParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|company| 单位  | body | false |string  |    |
|department| 部门  | body | false |string  |    |
|deviceCode| 设备唯一标识,主持人添加的deviceCode为空字符串，代表无位置  | body | false |string  |    |
|deviceType| 设备类型，主持人添加为null,可用值:CLIENT_DESKCARD,CLIENT_ONLY,DESKCARD_ONLY,CLIENT_DESKCARD_WITHOUT_DESKCARD,CLIENT_DESKCARD_WITHOUT_CLIENT,MOBILE_CLIENT,BIG_SCREEN,ROOMCARD,WEB  | body | false |string  |    |
|name| 姓名  | body | false |string  |    |
|post| 职位  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 添加或更新会议人员

**接口描述**:我的申请

**接口地址**:`/api/meeting_user/add_or_update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"department": "开发部",
	"departmentId": 1,
	"isHost": false,
	"mid": 1,
	"name": "超级管理员",
	"post": "普通员工",
	"signType": 1,
	"status": 1,
	"uid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserAddOrUpdateParam  | MeetingUserAddOrUpdateParam   |

**schema属性说明**



**MeetingUserAddOrUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|department| 部门  | body | false |string  |    |
|departmentId| 部门id  | body | false |integer(int64)  |    |
|isHost| 是否主持  | body | false |boolean  |    |
|mid| 会议id  | body | false |integer(int64)  |    |
|name| 姓名  | body | false |string  |    |
|post| 职位  | body | false |string  |    |
|signType| 签到类型  | body | false |integer(int32)  |    |
|status| 选中状态，0：未选中，1：选中  | body | false |integer(int32)  |    |
|uid| 人员id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 批量添加或更新会议人员

**接口描述**:我的申请

**接口地址**:`/api/meeting_user/add_or_updates`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"meetingUsers": [
		{
			"department": "开发部",
			"departmentId": 1,
			"isHost": false,
			"mid": 1,
			"name": "超级管理员",
			"post": "普通员工",
			"signType": 1,
			"status": 1,
			"uid": 1
		}
	]
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserAddOrUpdateListParam  | MeetingUserAddOrUpdateListParam   |

**schema属性说明**



**MeetingUserAddOrUpdateListParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|meetingUsers| 批量数据  | body | false |array  | MeetingUserAddOrUpdateParam   |

**MeetingUserAddOrUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|department| 部门  | body | false |string  |    |
|departmentId| 部门id  | body | false |integer(int64)  |    |
|isHost| 是否主持  | body | false |boolean  |    |
|mid| 会议id  | body | false |integer(int64)  |    |
|name| 姓名  | body | false |string  |    |
|post| 职位  | body | false |string  |    |
|signType| 签到类型  | body | false |integer(int32)  |    |
|status| 选中状态，0：未选中，1：选中  | body | false |integer(int32)  |    |
|uid| 人员id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取所有人员（包括未添加的）

**接口描述**:我的申请

**接口地址**:`/api/meeting_user/all_users`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"departmentId": 0,
	"mid": 1,
	"name": "张"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserAllUsersParam  | MeetingUserAllUsersParam   |

**schema属性说明**



**MeetingUserAllUsersParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|departmentId| 部门id,全部门为0  | body | false |integer(int64)  |    |
|mid| 会议id  | body | false |integer(int64)  |    |
|name| 与会者姓名  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"department": "",
			"departmentId": 0,
			"isHost": false,
			"mid": 0,
			"name": "",
			"post": "",
			"signType": 0,
			"status": 0,
			"uid": 0
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MeetingUserAllUsersDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingUserAllUsersDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|department | 部门   |string  |    |
|departmentId | 部门id   |integer(int64)  |    |
|isHost | 是否有主持权限   |boolean  |    |
|mid | 会议id   |integer(int64)  |    |
|name | 人员姓名   |string  |    |
|post | 职位   |string  |    |
|signType | 签到类型：0：免签，1：手写签到   |integer(int32)  |    |
|status | 是否选中,0：未选中，1：选中   |integer(int32)  |    |
|uid | 人员id   |integer(int64)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MeetingUserAllUsersDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 同屏权限申请

**接口描述**:客户端

**接口地址**:`/api/meeting_user/apply_srs`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"groupPre": "group_mid_",
	"muid": 1,
	"userPre": "muid_"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserApplySrsParam  | MeetingUserApplySrsParam   |

**schema属性说明**



**MeetingUserApplySrsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|groupPre| 通讯-组标识前缀  | body | false |string  |    |
|muid| 会议人员id（申请人）  | body | false |integer(int64)  |    |
|userPre| 通讯-人员标识前缀  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 同屏申请已处理通知其他主持人

**接口描述**:客户端

**接口地址**:`/api/meeting_user/apply_srs_setted`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"groupPre": "group_mid_",
	"muid": 1,
	"userPre": "muid_"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserApplySrsParam  | MeetingUserApplySrsParam   |

**schema属性说明**



**MeetingUserApplySrsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|groupPre| 通讯-组标识前缀  | body | false |string  |    |
|muid| 会议人员id（申请人）  | body | false |integer(int64)  |    |
|userPre| 通讯-人员标识前缀  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 人员排座

**接口描述**:我的申请

**接口地址**:`/api/meeting_user/bind_device`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"binds": [
		{
			"deviceId": 1,
			"muid": 1
		}
	],
	"mid": 1,
	"roomId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserBindDeviceParam  | MeetingUserBindDeviceParam   |

**schema属性说明**



**MeetingUserBindDeviceParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|binds| 会议人员与设备绑定List  | body | false |array  | MeetingUserBindDeviceSubParam   |
|mid| 会议id  | body | false |integer(int64)  |    |
|roomId| 会议室id  | body | false |integer(int64)  |    |

**MeetingUserBindDeviceSubParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|deviceId| 设备id  | body | false |integer(int64)  |    |
|muid| 会议人员id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 个人桌牌单刷


**接口描述**:


**接口地址**:`/api/meeting_user/deskcard_display`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|muid| muid  | query | true |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 桌牌客户端修改与会者信息

**接口描述**:客户端

**接口地址**:`/api/meeting_user/deskcard_info`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"name": "李建国",
	"post": "主任"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserDeskcardInfoParam  | MeetingUserDeskcardInfoParam   |

**schema属性说明**



**MeetingUserDeskcardInfoParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|name| 姓名  | body | false |string  |    |
|post| 职位  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 人脸识别签到

**接口描述**:客户端

**接口地址**:`/api/meeting_user/facerec_with_img`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |
|muid| muid  | query | false |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取人员排座

**接口描述**:我的申请,客户端

**接口地址**:`/api/meeting_user/get_binds`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1,
	"roomId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserGetBindsParam  | MeetingUserGetBindsParam   |

**schema属性说明**



**MeetingUserGetBindsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |
|roomId| 会议室id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"html": "",
		"meetingUsers": [
			{
				"deviceId": 0,
				"muid": 0,
				"name": "",
				"number": 0
			}
		]
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |MeetingUserGetBindsDTO  | MeetingUserGetBindsDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingUserGetBindsDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|html | 会议室硬件   |string  |    |
|meetingUsers | 会议人员设备绑定数组   |array  | MeetingUserGetBindsSubDTO   |

**MeetingUserGetBindsSubDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|deviceId | 设备id   |integer(int64)  |    |
|muid | 会议人员Id   |integer(int64)  |    |
|name | 姓名   |string  |    |
|number | 座位号   |integer(int32)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«MeetingUserGetBindsDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 导出临时与会者excel


**接口描述**:


**接口地址**:`/api/meeting_user/get_excel/{mid}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["application/octet-stream"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|mid| mid  | path | true |integer  |    |

**响应示例**:

```json

```

**响应参数**:


暂无





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取会议室人员排座


**接口描述**:


**接口地址**:`/api/meeting_user/get_room_binds`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"roomId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserGetRoomBindsParam  | MeetingUserGetRoomBindsParam   |

**schema属性说明**



**MeetingUserGetRoomBindsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|roomId| 会议室id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"html": "",
		"meetingUsers": [
			{
				"deviceId": 0,
				"muid": 0,
				"name": "",
				"number": 0
			}
		]
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |MeetingUserGetBindsDTO  | MeetingUserGetBindsDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingUserGetBindsDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|html | 会议室硬件   |string  |    |
|meetingUsers | 会议人员设备绑定数组   |array  | MeetingUserGetBindsSubDTO   |

**MeetingUserGetBindsSubDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|deviceId | 设备id   |integer(int64)  |    |
|muid | 会议人员Id   |integer(int64)  |    |
|name | 姓名   |string  |    |
|number | 座位号   |integer(int32)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«MeetingUserGetBindsDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议人员基本信息

**接口描述**:客户端

**接口地址**:`/api/meeting_user/info`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"department": "",
		"deviceId": 0,
		"deviceNum": 0,
		"isHost": false,
		"isSigned": false,
		"isSrsDisabled": false,
		"meetingName": "",
		"mid": 0,
		"muid": 0,
		"name": "",
		"post": "",
		"room": "",
		"roomId": 0,
		"signImg": "",
		"signTime": "",
		"signType": 0,
		"time": ""
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |MeetingUserInfoDTO  | MeetingUserInfoDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingUserInfoDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|department | 部门   |string  |    |
|deviceId | 设备id   |integer(int64)  |    |
|deviceNum | 设备号   |integer(int32)  |    |
|isHost | 是否主持人   |boolean  |    |
|isSigned | 是否已签到   |boolean  |    |
|isSrsDisabled | 同屏禁用   |boolean  |    |
|meetingName | 会议名称   |string  |    |
|mid | 会议id   |integer(int64)  |    |
|muid | 会议人员id   |integer(int64)  |    |
|name | 用户名   |string  |    |
|post | 职位   |string  |    |
|room | 会议室名称   |string  |    |
|roomId | 会议室id   |integer(int64)  |    |
|signImg | 签到图片   |string  |    |
|signTime | 签到时间   |string(date-time)  |    |
|signType | 签到类型   |integer(int32)  |    |
|time | 时间   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«MeetingUserInfoDTO»|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 是否提示签到

**接口描述**:客户端

**接口地址**:`/api/meeting_user/is_need_sign`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取会议人员(by mid)


**接口描述**:


**接口地址**:`/api/meeting_user/list`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"department": "",
			"departmentId": 0,
			"id": 0,
			"isEmailed": false,
			"isGuest": false,
			"isHost": false,
			"isSigned": false,
			"isSmsed": false,
			"isSrsDisabled": false,
			"isWechated": false,
			"name": "",
			"post": "",
			"signImg": "",
			"signTime": "",
			"signType": 0
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MeetingUserListDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingUserListDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|department | 部门   |string  |    |
|departmentId | 部门id   |integer(int64)  |    |
|id | 会议人员id   |integer(int64)  |    |
|isEmailed | 邮箱已通知   |boolean  |    |
|isGuest | 是否来宾   |boolean  |    |
|isHost | 是否主持人   |boolean  |    |
|isSigned | 是否已签到   |boolean  |    |
|isSmsed | 短信已通知   |boolean  |    |
|isSrsDisabled | 是否禁用同屏   |boolean  |    |
|isWechated | 微信已通知   |boolean  |    |
|name | 姓名   |string  |    |
|post | 职位   |string  |    |
|signImg | 签到图片   |string  |    |
|signTime | 签到时间   |string(date-time)  |    |
|signType | 签到类型   |integer(int32)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MeetingUserListDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议桌牌群刷


**接口描述**:


**接口地址**:`/api/meeting_user/meeting_deskcard_displays`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|mid| mid  | query | true |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议室-重置同屏


**接口描述**:


**接口地址**:`/api/meeting_user/reset_room_srs`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|roomId| roomId  | query | true |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取会议室人员排座（移动终端用-四川中烟扫码选人）


**接口描述**:


**接口地址**:`/api/meeting_user/room_binds`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1,
	"roomId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserGetBindsParam  | MeetingUserGetBindsParam   |

**schema属性说明**



**MeetingUserGetBindsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |
|roomId| 会议室id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"deviceId": 0,
			"muid": 0,
			"name": "",
			"number": 0
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MeetingUserGetBindsSubDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingUserGetBindsSubDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|deviceId | 设备id   |integer(int64)  |    |
|muid | 会议人员Id   |integer(int64)  |    |
|name | 姓名   |string  |    |
|number | 座位号   |integer(int32)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MeetingUserGetBindsSubDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 设置启/禁用同屏（可选通知申请人）

**接口描述**:客户端

**接口地址**:`/api/meeting_user/set_srs`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"groupPre": "group_mid_",
	"isSrsDisabled": true,
	"muid": 1,
	"userPre": "muid_"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserSetSrsParam  | MeetingUserSetSrsParam   |

**schema属性说明**



**MeetingUserSetSrsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|groupPre| 通讯-组标识前缀  | body | false |string  |    |
|isSrsDisabled| 是否禁用  | body | false |boolean  |    |
|muid| 会议人员id  | body | false |integer(int64)  |    |
|userPre| 通讯-人员标识前缀  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 全部启/禁用同屏

**接口描述**:客户端

**接口地址**:`/api/meeting_user/set_srs_all`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"groupPre": "group_mid_",
	"isSrsDisabled": true,
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserSetSrsAllParam  | MeetingUserSetSrsAllParam   |

**schema属性说明**



**MeetingUserSetSrsAllParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|groupPre| 通讯-组标识前缀  | body | false |string  |    |
|isSrsDisabled| 是否禁用  | body | false |boolean  |    |
|mid| 会议人员id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 下载会议签到Excel


**接口描述**:


**接口地址**:`/api/meeting_user/sign_excel/{mid}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["application/octet-stream"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|mid| mid  | path | true |integer  |    |

**响应示例**:

```json

```

**响应参数**:


暂无





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 手机扫码签到


**接口描述**:


**接口地址**:`/api/meeting_user/sign_scan`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"account": "user",
	"mid": 1,
	"password": "123"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserSignScanParam  | MeetingUserSignScanParam   |

**schema属性说明**



**MeetingUserSignScanParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|account| 账户名  | body | false |string  |    |
|mid| 会议id  | body | false |integer(int64)  |    |
|password| 密码  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 政务微信扫码签到


**接口描述**:


**接口地址**:`/api/meeting_user/sign_wechat_scan`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1,
	"openId": "5e4109f475c1bd50781d6124"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserSignWechatScanParam  | MeetingUserSignWechatScanParam   |

**schema属性说明**



**MeetingUserSignWechatScanParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |
|openId| wechat-OpenId  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 手机扫码输入手机号签到


**接口描述**:


**接口地址**:`/api/meeting_user/sign_with_mobile`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1,
	"mobile": "132xxxx5780"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserSignWithMobileParam  | MeetingUserSignWithMobileParam   |

**schema属性说明**



**MeetingUserSignWithMobileParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | true |integer(int64)  |    |
|mobile| 手机号  | body | true |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 手写图片签到base64

**接口描述**:客户端

**接口地址**:`/api/meeting_user/signin_with_base64`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|fileStr| fileStr  | query | false |string  |    |
|muid| muid  | query | false |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 手写图片签到

**接口描述**:客户端

**接口地址**:`/api/meeting_user/signin_with_img`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |
|muid| muid  | query | false |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取会议人员字符串

**接口描述**:我的申请

**接口地址**:`/api/meeting_user/str`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 导入临时与会者excel


**接口描述**:


**接口地址**:`/api/meeting_user/upload_excel/{mid}`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|mid| mid  | path | true |integer  |    |
|file| file  | formData | true |file  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取会议人员（已添加的）


**接口描述**:


**接口地址**:`/api/meeting_user/users`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1,
	"name": "张"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingUserUsersParam  | MeetingUserUsersParam   |

**schema属性说明**



**MeetingUserUsersParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |
|name| 与会者姓名  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"department": "",
			"departmentId": 0,
			"isHost": false,
			"mid": 0,
			"name": "",
			"post": "",
			"signType": 0,
			"status": 0,
			"uid": 0
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MeetingUserAllUsersDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingUserAllUsersDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|department | 部门   |string  |    |
|departmentId | 部门id   |integer(int64)  |    |
|isHost | 是否有主持权限   |boolean  |    |
|mid | 会议id   |integer(int64)  |    |
|name | 人员姓名   |string  |    |
|post | 职位   |string  |    |
|signType | 签到类型：0：免签，1：手写签到   |integer(int32)  |    |
|status | 是否选中,0：未选中，1：选中   |integer(int32)  |    |
|uid | 人员id   |integer(int64)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MeetingUserAllUsersDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# meeting_chat

## 会议聊天


**接口描述**:


**接口地址**:`/api/meeting_chat/action`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"content": "消息内容",
	"groupPre": "group_mid_",
	"mid": 1,
	"receiverMuid": 0,
	"sender": "人员名称/设备名称",
	"senderMuid": 1,
	"userPre": "muid_"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingChatActionParam  | MeetingChatActionParam   |

**schema属性说明**



**MeetingChatActionParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|content| 消息内容  | body | false |string  |    |
|groupPre| 通讯-组标识前缀  | body | false |string  |    |
|mid| 会议id  | body | false |integer(int64)  |    |
|receiverMuid| 接收人会议人员id，群发为0, 呼叫服务为'-99'  | body | false |integer(int64)  |    |
|sender| 发送人名称  | body | false |string  |    |
|senderMuid| 发送人会议人员id  | body | false |integer(int64)  |    |
|userPre| 通讯-人员标识前缀  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 删除会议聊天记录（服务中心mid：-99）


**接口描述**:


**接口地址**:`/api/meeting_chat/delete_by_mid`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingIdParam  | MeetingIdParam   |

**schema属性说明**



**MeetingIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议聊天记录


**接口描述**:


**接口地址**:`/api/meeting_chat/list`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"myMuid": 1,
	"otherMuid": 2
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingChatListParam  | MeetingChatListParam   |

**schema属性说明**



**MeetingChatListParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|myMuid| 我的会议人员id  | body | false |integer(int64)  |    |
|otherMuid| 他人会议人员id, 会议群聊为0  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"content": "",
			"gmtCreate": "",
			"isMyself": false,
			"receiverMuid": 0,
			"sender": "",
			"senderMuid": 0
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MeetingChatListDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingChatListDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|content | 发送具体内容   |string  |    |
|gmtCreate | 发送时间   |string(date-time)  |    |
|isMyself | 是否自己的信息   |boolean  |    |
|receiverMuid | 接受人的会议人员id   |integer(int64)  |    |
|sender | 发送人名称   |string  |    |
|senderMuid | 发送人的会议人员id   |integer(int64)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MeetingChatListDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 下载会议聊天记录Excel


**接口描述**:


**接口地址**:`/api/meeting_chat/meeting_chat_excel/{mid}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["application/octet-stream"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|mid| mid  | path | true |integer  |    |

**响应示例**:

```json

```

**响应参数**:


暂无





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# media_service

## 新增流媒体服务器

**接口描述**:服务器列表

**接口地址**:`/api/media_service/add`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"apiPort": 1985,
	"internalIp": "192.168.1.11",
	"internalPort": 1935,
	"ip": "192.168.0.11",
	"isHost": false,
	"port": 1935,
	"remark": "备注信息"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MediaServiceAddParam  | MediaServiceAddParam   |

**schema属性说明**



**MediaServiceAddParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|apiPort| api状态监测端口号  | body | false |integer(int32)  |    |
|internalIp| 内部映射ip地址  | body | false |string  |    |
|internalPort| 内部映射端口号  | body | false |integer(int32)  |    |
|ip| 服务器映射ip地址  | body | false |string  |    |
|isHost| 是否主服务器  | body | false |boolean  |    |
|port| 服务器映射端口号  | body | false |integer(int32)  |    |
|remark| 备注  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 是否他人已控制'远程画面'

**接口描述**:客户端

**接口地址**:`/api/media_service/is_other_vp_opened`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"isOpen": false,
		"isSelfOpen": false
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |MediaServiceIsVpOpenedDTO  | MediaServiceIsVpOpenedDTO   |
|message|   |string  |    |



**schema属性说明**




**MediaServiceIsVpOpenedDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|isOpen | 是否已打开   |boolean  |    |
|isSelfOpen | 是否是自己打开   |boolean  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«MediaServiceIsVpOpenedDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 流媒体列表

**接口描述**:服务器列表

**接口地址**:`/api/media_service/list`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"apiPort": 0,
			"id": 0,
			"internalIp": "",
			"internalPort": 0,
			"ip": "",
			"isHost": false,
			"isOnline": false,
			"port": 0,
			"remark": "",
			"statusEnum": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MediaServiceStatusDTO   |
|message|   |string  |    |



**schema属性说明**




**MediaServiceStatusDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|apiPort | api状态监测端口号   |integer(int32)  |    |
|id | 流媒体id   |integer(int64)  |    |
|internalIp | 内部映射ip地址   |string  |    |
|internalPort | 内部映射端口号   |integer(int32)  |    |
|ip | 服务器映射ip地址   |string  |    |
|isHost | 是否主服务器   |boolean  |    |
|isOnline | 是否在线   |boolean  |    |
|port | 服务器映射端口号   |integer(int32)  |    |
|remark | 备注   |string  |    |
|statusEnum | 启/禁用状态,可用值:ENABLED,DISABLED,DELETED   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MediaServiceStatusDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 启用/禁用/删除

**接口描述**:服务器列表

**接口地址**:`/api/media_service/modify_status`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1,
	"status": "ENABLED"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |CommonStatusParam  | CommonStatusParam   |

**schema属性说明**



**CommonStatusParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |
|status| 状态( ENABLED(1, "已启用"), DISABLED(0, "已禁用"), DELETED(-1, "已删除");),可用值:ENABLED,DISABLED,DELETED  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 统一打开'远程画面'

**接口描述**:客户端

**接口地址**:`/api/media_service/open_video_progress`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"action": "open",
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MediaServiceOpenVideoProgressParam  | MediaServiceOpenVideoProgressParam   |

**schema属性说明**



**MediaServiceOpenVideoProgressParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|action| 操作类型'open/close'  | body | false |string  |    |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 重置控制'远程画面'


**接口描述**:


**接口地址**:`/api/media_service/reset_vp_opened/{mid}`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|mid| mid  | path | true |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取拉流（播放）地址

**接口描述**:客户端

**接口地址**:`/api/media_service/rtmp_pull_url`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"deviceTypeEnum": "",
	"key": "fd4aa65b336fabffd5f6b16ac6505861e32fde3c9fcbdbdfb99501a9affb2a52"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MediaServiceRtmpPullUrlParam  | MediaServiceRtmpPullUrlParam   |

**schema属性说明**



**MediaServiceRtmpPullUrlParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|deviceTypeEnum| 设备类型,可用值:CLIENT_DESKCARD,CLIENT_ONLY,DESKCARD_ONLY,CLIENT_DESKCARD_WITHOUT_DESKCARD,CLIENT_DESKCARD_WITHOUT_CLIENT,MOBILE_CLIENT,BIG_SCREEN,ROOMCARD,WEB  | body | false |string  |    |
|key| 直播key  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取推流地址

**接口描述**:客户端

**接口地址**:`/api/media_service/rtmp_push_url`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"deviceTypeEnumList": [],
	"groupId": "group_mid_1"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MediaServiceRtmpPushUrlParam  | MediaServiceRtmpPushUrlParam   |

**schema属性说明**



**MediaServiceRtmpPushUrlParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|deviceTypeEnumList| 接收同屏信号设备类型,可用值:CLIENT_DESKCARD,CLIENT_ONLY,DESKCARD_ONLY,CLIENT_DESKCARD_WITHOUT_DESKCARD,CLIENT_DESKCARD_WITHOUT_CLIENT,MOBILE_CLIENT,BIG_SCREEN,ROOMCARD,WEB  | body | false |array  |    |
|groupId| 组id,无纸化客户端约定为 'group_mid_' + 会议id  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 发送统一打开视频消息

**接口描述**:客户端

**接口地址**:`/api/media_service/send_open_video`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1,
	"videoUrl": "rtmp://192.168.0.249:1935/room_video/test"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MediaServiceSendOpenVideoParam  | MediaServiceSendOpenVideoParam   |

**schema属性说明**



**MediaServiceSendOpenVideoParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |
|videoUrl| 视频地址  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 二次加入同屏,发送同屏发起人消息


**接口描述**:


**接口地址**:`/api/media_service/send_pusher`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"name": "",
	"stream": ""
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MediaSendPusherParam  | MediaSendPusherParam   |

**schema属性说明**



**MediaSendPusherParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|name| 二次加入同屏用户名称  | body | true |string  |    |
|stream| 同屏的流信息  | body | true |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 查询流媒体状态

**接口描述**:服务器列表

**接口地址**:`/api/media_service/status`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"apiPort": 0,
			"id": 0,
			"internalIp": "",
			"internalPort": 0,
			"ip": "",
			"isHost": false,
			"isOnline": false,
			"port": 0,
			"remark": "",
			"statusEnum": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MediaServiceStatusDTO   |
|message|   |string  |    |



**schema属性说明**




**MediaServiceStatusDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|apiPort | api状态监测端口号   |integer(int32)  |    |
|id | 流媒体id   |integer(int64)  |    |
|internalIp | 内部映射ip地址   |string  |    |
|internalPort | 内部映射端口号   |integer(int32)  |    |
|ip | 服务器映射ip地址   |string  |    |
|isHost | 是否主服务器   |boolean  |    |
|isOnline | 是否在线   |boolean  |    |
|port | 服务器映射端口号   |integer(int32)  |    |
|remark | 备注   |string  |    |
|statusEnum | 启/禁用状态,可用值:ENABLED,DISABLED,DELETED   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MediaServiceStatusDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 更新流媒体服务器

**接口描述**:服务器列表

**接口地址**:`/api/media_service/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"apiPort": 1985,
	"id": 1,
	"internalIp": "192.168.0.101",
	"internalPort": 1935,
	"ip": "192.168.0.101",
	"isHost": false,
	"port": 1935,
	"remark": "备注信息"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MediaServiceUpdateParam  | MediaServiceUpdateParam   |

**schema属性说明**



**MediaServiceUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|apiPort| api状态监测端口号  | body | false |integer(int32)  |    |
|id| 服务器id  | body | false |integer(int64)  |    |
|internalIp| 内部映射ip地址  | body | false |string  |    |
|internalPort| 内部映射端口号  | body | false |integer(int32)  |    |
|ip| 服务器映射ip地址  | body | false |string  |    |
|isHost| 是否主服务器  | body | false |boolean  |    |
|port| 服务器映射端口号  | body | false |integer(int32)  |    |
|remark| 备注  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## '远程画面'进度信息同步

**接口描述**:客户端

**接口地址**:`/api/media_service/update_video_progress`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"muid": 1,
	"progress": "0.251",
	"videoUrl": "http://192.168.0.249/room_video/test.mp4"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MediaServiceUpdateVideoProgressParam  | MediaServiceUpdateVideoProgressParam   |

**schema属性说明**



**MediaServiceUpdateVideoProgressParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|muid| 会议人员id  | body | false |integer(int64)  |    |
|progress| 视频进度  | body | false |string  |    |
|videoUrl| 视频地址  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## '远程画面'操作同步

**接口描述**:客户端

**接口地址**:`/api/media_service/vp_action`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"action": "play",
	"muid": 1,
	"volumeNum": 0.5236
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MediaServiceVpActionParam  | MediaServiceVpActionParam   |

**schema属性说明**



**MediaServiceVpActionParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|action| 操作类型'play/pause/volume'  | body | false |string  |    |
|muid| 会议人员id  | body | false |integer(int64)  |    |
|volumeNum| 声音数值（0-1）,声音操作有效  | body | false |number(double)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# ppt

## 打开主持人演示的office


**接口描述**:


**接口地址**:`/api/ppt/follow_view`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|mid| 会议id  | query | false |integer  |    |
|muid| 会议人员id  | query | false |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 打开原生office


**接口描述**:


**接口地址**:`/api/ppt/view`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|fileId| 文件Id  | query | false |integer  |    |
|mid| 会议id  | query | false |integer  |    |
|muid| 会议人员id  | query | false |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# permission

## 查询系统所有菜单(系统信息，无需关注)


**接口描述**:


**接口地址**:`/api/permission/system_menus`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 服务器基本信息


**接口描述**:


**接口地址**:`/api/permission/system_number`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 查询系统所有权限(系统信息，无需关注)


**接口描述**:


**接口地址**:`/api/permission/system_permissions`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# process

## 获取会议审核进度列表

**接口描述**:我的申请,会议审核

**接口地址**:`/api/process/meeting`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"content": "",
			"createUid": 0,
			"createUser": "",
			"gmtCreate": "",
			"status": 0
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | ProcessMeetingDTO   |
|message|   |string  |    |



**schema属性说明**




**ProcessMeetingDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|content | 进度详情   |string  |    |
|createUid | 进度编辑人id   |integer(int64)  |    |
|createUser | 进度编辑人   |string  |    |
|gmtCreate | 时间   |string(date-time)  |    |
|status | 进度状态，1：当前进度 0：非当前   |integer(int32)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«ProcessMeetingDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## (我的)会议审核

**接口描述**:会议审核

**接口地址**:`/api/process/my_review`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"id": 0,
			"mid": 0,
			"title": "",
			"time": "",
			"room": "",
			"remark": "",
			"department": "",
			"name": "",
			"mobile": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | ProcessMyReviewDTO   |
|message|   |string  |    |



**schema属性说明**




**ProcessMyReviewDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|id | 进度id   |integer(int64)  |    |
|mid | 会议id   |integer(int64)  |    |
|title | 会议标题   |string  |    |
|time | 会议时间   |string  |    |
|room | 会议地点   |string  |    |
|remark | 会议备注   |string  |    |
|department | 部门   |string  |    |
|name | 姓名   |string  |    |
|mobile | 电话   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«ProcessMyReviewDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 审核操作

**接口描述**:会议审核

**接口地址**:`/api/process/review`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"content": "同意申请",
	"mid": 1,
	"processId": 0,
	"reviewUid": 2,
	"type": "AGREE"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |ProcessReviewParam  | ProcessReviewParam   |

**schema属性说明**



**ProcessReviewParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|content| 审核回馈内容  | body | false |string  |    |
|mid| 会议id  | body | false |integer(int64)  |    |
|processId| 审核id  | body | false |integer(int64)  |    |
|reviewUid| 转发人员id  | body | false |integer(int64)  |    |
|type| 操作类型, AGREE:同意，SEND：转发，REFUSE：拒绝,可用值:AGREE,SEND,REFUSE  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# role

## 新增角色

**接口描述**:角色管理

**接口地址**:`/api/role/add`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"name": "管理员"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |RoleAddParam  | RoleAddParam   |

**schema属性说明**



**RoleAddParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|name| 角色名称  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 所有角色

**接口描述**:角色管理

**接口地址**:`/api/role/all`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"gmtCreate": "",
			"gmtModify": "",
			"id": 0,
			"name": "",
			"status": "",
			"statusEnum": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | RoleAllDTO   |
|message|   |string  |    |



**schema属性说明**




**RoleAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|gmtCreate | 创建时间   |string(date-time)  |    |
|gmtModify | 最后编辑   |string(date-time)  |    |
|id | 角色id   |integer(int64)  |    |
|name | 角色名   |string  |    |
|status | 角色状态   |string  |    |
|statusEnum | 角色状态,可用值:ENABLED,DISABLED,DELETED   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«RoleAllDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 已启用角色

**接口描述**:人员列表

**接口地址**:`/api/role/enableds`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"id": 0,
			"name": "",
			"status": 0
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | RoleEnabledsDTO   |
|message|   |string  |    |



**schema属性说明**




**RoleEnabledsDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|id | 角色id   |integer(int64)  |    |
|name | 角色名   |string  |    |
|status | 角色状态，0：禁用，1：启用   |integer(int32)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«RoleEnabledsDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 启用/禁用/删除

**接口描述**:角色管理

**接口地址**:`/api/role/modify_status`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1,
	"status": "ENABLED"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |CommonStatusParam  | CommonStatusParam   |

**schema属性说明**



**CommonStatusParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |
|status| 状态( ENABLED(1, "已启用"), DISABLED(0, "已禁用"), DELETED(-1, "已删除");),可用值:ENABLED,DISABLED,DELETED  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 根据token获取角色菜单

**接口描述**:公共必须

**接口地址**:`/api/role/navs`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"children": [
				{
					"children": [
						{}
					],
					"icon": "",
					"id": "",
					"isChecked": false,
					"pid": "",
					"title": "",
					"url": ""
				}
			],
			"icon": "",
			"id": "",
			"isChecked": false,
			"pid": "",
			"title": "",
			"url": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | TreeNode   |
|message|   |string  |    |



**schema属性说明**




**TreeNode**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|children | 子项   |array  | TreeNode   |
|icon | 图标   |string  |    |
|id | 主键标识   |string  |    |
|isChecked | 是否选中   |boolean  |    |
|pid | 父标识   |string  |    |
|title | 标题   |string  |    |
|url | 路径   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«TreeNode»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 查看角色菜单选中情况（编辑页）

**接口描述**:角色管理

**接口地址**:`/api/role/navs_setted`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"roleId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |RoleIdParam  | RoleIdParam   |

**schema属性说明**



**RoleIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|roleId| 角色id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"children": [
				{
					"children": [
						{}
					],
					"icon": "",
					"id": "",
					"isChecked": false,
					"pid": "",
					"title": "",
					"url": ""
				}
			],
			"icon": "",
			"id": "",
			"isChecked": false,
			"pid": "",
			"title": "",
			"url": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | TreeNode   |
|message|   |string  |    |



**schema属性说明**




**TreeNode**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|children | 子项   |array  | TreeNode   |
|icon | 图标   |string  |    |
|id | 主键标识   |string  |    |
|isChecked | 是否选中   |boolean  |    |
|pid | 父标识   |string  |    |
|title | 标题   |string  |    |
|url | 路径   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«TreeNode»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 修改角色

**接口描述**:角色管理

**接口地址**:`/api/role/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1,
	"name": "管理员"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |RoleUpdateParam  | RoleUpdateParam   |

**schema属性说明**



**RoleUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 角色id  | body | false |integer(int64)  |    |
|name| 角色名称  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 更新角色菜单

**接口描述**:角色管理

**接口地址**:`/api/role/update_navs`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"menus": [],
	"roleId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |RoleUpdateNavsParam  | RoleUpdateNavsParam   |

**schema属性说明**



**RoleUpdateNavsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|menus| 菜单数组  | body | false |array  |    |
|roleId| 角色id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# room

## 新增

**接口描述**:会议室

**接口地址**:`/api/room/add`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"classId": 1,
	"remark": "备注",
	"seatNum": 30,
	"title": "703室"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |RoomAddParam  | RoomAddParam   |

**schema属性说明**



**RoomAddParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|classId| 会议室分区id  | body | false |integer(int64)  |    |
|remark| 备注  | body | false |string  |    |
|seatNum| 座位数  | body | false |integer(int32)  |    |
|title| 会议室名称  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 所有会议室

**接口描述**:会议室

**接口地址**:`/api/room/all`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"classId": 0,
			"classTitle": "",
			"com": "",
			"downCode": "",
			"gmtCreate": "",
			"gmtModify": "",
			"id": 0,
			"remark": "",
			"seatNum": 0,
			"serialPortDeviceCode": "",
			"status": 0,
			"stopCode": "",
			"title": "",
			"upCode": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | RoomAllDTO   |
|message|   |string  |    |



**schema属性说明**




**RoomAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|classId | 会议室分区id   |integer(int64)  |    |
|classTitle | 分区名称   |string  |    |
|com | COM口   |string  |    |
|downCode | 统一下降码   |string  |    |
|gmtCreate | 创建时间   |string(date-time)  |    |
|gmtModify | 最后编辑   |string(date-time)  |    |
|id | 会议室id   |integer(int64)  |    |
|remark | 备注   |string  |    |
|seatNum | 设备数目   |integer(int32)  |    |
|serialPortDeviceCode | 串口设备唯一标识   |string  |    |
|status | 状态，0:已禁用，1:已启用   |integer(int32)  |    |
|stopCode | 统一停止码   |string  |    |
|title | 会议室名称   |string  |    |
|upCode | 统一上升码   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«RoomAllDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 所有分区会议室列表


**接口描述**:


**接口地址**:`/api/room/class_enabled`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"no": 0,
			"rooms": [
				{
					"classId": 0,
					"currentMid": 0,
					"id": 0,
					"seatNum": 0,
					"title": ""
				}
			],
			"title": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | RoomClassEnabledDTO   |
|message|   |string  |    |



**schema属性说明**




**RoomClassEnabledDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|no | 分区序号   |integer(int32)  |    |
|rooms | 分区会议室   |array  | RoomClassEnabledSubDTO   |
|title | 分区名称   |string  |    |

**RoomClassEnabledSubDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|classId | 会议室分区id   |integer(int64)  |    |
|currentMid | 当前正在召开的会议id   |integer(int64)  |    |
|id | 会议室id   |integer(int64)  |    |
|seatNum | 座位数   |integer(int32)  |    |
|title | 会议室名称   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«RoomClassEnabledDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 统一升降开关机


**接口描述**:


**接口地址**:`/api/room/device_action`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"roomId": 1,
	"typeEnum": "UP"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |RoomDeviceActionParam  | RoomDeviceActionParam   |

**schema属性说明**



**RoomDeviceActionParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|roomId| 会议室id  | body | false |integer(int64)  |    |
|typeEnum| 操作类型,可用值:UP,DOWN,STOP,CLOSE,OPEN  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 可用会议室

**接口描述**:我的申请

**接口地址**:`/api/room/enabled`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"classId": 0,
			"classTitle": "",
			"com": "",
			"downCode": "",
			"gmtCreate": "",
			"gmtModify": "",
			"id": 0,
			"remark": "",
			"seatNum": 0,
			"serialPortDeviceCode": "",
			"status": 0,
			"stopCode": "",
			"title": "",
			"upCode": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | RoomAllDTO   |
|message|   |string  |    |



**schema属性说明**




**RoomAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|classId | 会议室分区id   |integer(int64)  |    |
|classTitle | 分区名称   |string  |    |
|com | COM口   |string  |    |
|downCode | 统一下降码   |string  |    |
|gmtCreate | 创建时间   |string(date-time)  |    |
|gmtModify | 最后编辑   |string(date-time)  |    |
|id | 会议室id   |integer(int64)  |    |
|remark | 备注   |string  |    |
|seatNum | 设备数目   |integer(int32)  |    |
|serialPortDeviceCode | 串口设备唯一标识   |string  |    |
|status | 状态，0:已禁用，1:已启用   |integer(int32)  |    |
|stopCode | 统一停止码   |string  |    |
|title | 会议室名称   |string  |    |
|upCode | 统一上升码   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«RoomAllDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取会议室硬件设施

**接口描述**:会议室

**接口地址**:`/api/room/hardware`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"roomId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |RoomHardwareParam  | RoomHardwareParam   |

**schema属性说明**



**RoomHardwareParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|roomId| 会议室id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"devices": [
			{
				"id": 0,
				"number": 0,
				"typeEnum": ""
			}
		],
		"html": "",
		"otherDevices": [
			{
				"id": 0,
				"number": 0,
				"typeEnum": ""
			}
		]
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |RoomHardwareDTO  | RoomHardwareDTO   |
|message|   |string  |    |



**schema属性说明**




**RoomHardwareDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|devices | 会议室在用设备信息   |array  | RoomHardware_deviceDTO   |
|html | 会议室硬件设备html   |string  |    |
|otherDevices | 会议室可用设备信息   |array  | RoomHardware_deviceDTO   |

**RoomHardware_deviceDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|id | 设备id   |integer(int64)  |    |
|number | 设备号   |integer(int32)  |    |
|typeEnum | 设备类型,可用值:CLIENT_DESKCARD,CLIENT_ONLY,DESKCARD_ONLY,CLIENT_DESKCARD_WITHOUT_DESKCARD,CLIENT_DESKCARD_WITHOUT_CLIENT,MOBILE_CLIENT,BIG_SCREEN,ROOMCARD,WEB   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«RoomHardwareDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 更新会议室硬件设施

**接口描述**:会议室

**接口地址**:`/api/room/hardware_update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"devices": [
		{
			"id": 1
		}
	],
	"html": "html",
	"roomId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |RoomHardwareUpdateParam  | RoomHardwareUpdateParam   |

**schema属性说明**



**RoomHardwareUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|devices| 设备数组  | body | false |array  | RoomHardwareUpdate_deviceParam   |
|html| 会议室会议室硬件设施页面  | body | false |string  |    |
|roomId| 会议室id  | body | false |integer(int64)  |    |

**RoomHardwareUpdate_deviceParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 设备id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 启用/禁用/删除

**接口描述**:会议室

**接口地址**:`/api/room/modify_status`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1,
	"status": "ENABLED"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |CommonStatusParam  | CommonStatusParam   |

**schema属性说明**



**CommonStatusParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |
|status| 状态( ENABLED(1, "已启用"), DISABLED(0, "已禁用"), DELETED(-1, "已删除");),可用值:ENABLED,DISABLED,DELETED  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议室二维码


**接口描述**:


**接口地址**:`/api/room/qrcode/{roomId}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["application/octet-stream"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|roomId| roomId  | path | true |integer  |    |

**响应示例**:

```json

```

**响应参数**:


暂无





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 单个会议签到二维码


**接口描述**:


**接口地址**:`/api/room/qrcode/{roomId}/mid/{mid}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["application/octet-stream"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|roomId| roomId  | path | true |integer  |    |
|mid| mid  | path | true |integer  |    |

**响应示例**:

```json

```

**响应参数**:


暂无





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 刷新门牌灯带


**接口描述**:


**接口地址**:`/api/room/redload_roomcard_light/{roomId}`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|roomId| roomId  | path | true |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 分区会议室列表

**接口描述**:设置会议

**接口地址**:`/api/room/select_by_class_id`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"roomClassId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |RoomSelectByClassIdParam  | RoomSelectByClassIdParam   |

**schema属性说明**



**RoomSelectByClassIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|roomClassId| 会议室分区id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"classTitle": "",
			"remark": "",
			"roomId": 0,
			"seatNum": 0,
			"title": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | RoomSelectByClassIdDTO   |
|message|   |string  |    |



**schema属性说明**




**RoomSelectByClassIdDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|classTitle | 会议室分区标题   |string  |    |
|remark | 备注   |string  |    |
|roomId | 会议室id   |integer(int64)  |    |
|seatNum | 会议室座位数   |integer(int32)  |    |
|title | 会议室标题   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«RoomSelectByClassIdDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 设置会议室的当前会议

**接口描述**:设置会议,客户端

**接口地址**:`/api/room/set_current_meeting`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1,
	"roomId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |RoomSetCurrentMeetingParam  | RoomSetCurrentMeetingParam   |

**schema属性说明**



**RoomSetCurrentMeetingParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |
|roomId| 会议室roomId  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 更新

**接口描述**:会议室

**接口地址**:`/api/room/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"classId": 1,
	"com": "COM3",
	"downCode": "D6 F2 FE",
	"id": 1,
	"remark": "备注",
	"seatNum": 30,
	"serialPortDeviceCode": "devicecode",
	"stopCode": "D6 F3 FE",
	"title": "703室",
	"upCode": "D6 F1 FE"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |RoomUpdateParam  | RoomUpdateParam   |

**schema属性说明**



**RoomUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|classId| 会议室分区id  | body | false |integer(int64)  |    |
|com| com口  | body | false |string  |    |
|downCode| 下降码  | body | false |string  |    |
|id| 会议室id  | body | false |integer(int64)  |    |
|remark| 备注  | body | false |string  |    |
|seatNum| 座位数目  | body | false |integer(int32)  |    |
|serialPortDeviceCode| 串口设备唯一标识码  | body | false |string  |    |
|stopCode| 停止码  | body | false |string  |    |
|title| 会议室名称  | body | false |string  |    |
|upCode| 上升码  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议室使用情况


**接口描述**:


**接口地址**:`/api/room/used_detail`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"date": "2020-11-23"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |RoomUsedDetailParam  | RoomUsedDetailParam   |

**schema属性说明**



**RoomUsedDetailParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|date| 查询日期  | body | false |string(date-time)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"effectives": [
				{
					"clock": 0,
					"effectiveEnum": "",
					"end": 0,
					"start": 0
				}
			],
			"roomId": 0,
			"title": "",
			"classTitle": "",
			"seatNum": 0,
			"times": [
				{
					"statusEnum": "",
					"meeting": "",
					"startTime": "",
					"startStamp": 0,
					"endTime": "",
					"endStamp": 0
				}
			]
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | RoomUsedDetailDTO   |
|message|   |string  |    |



**schema属性说明**




**RoomUsedDetailDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|effectives | 占用格子15min, 24hx4   |array  | RoomUsedEffectiveDTO   |
|roomId | 会议室id   |integer(int64)  |    |
|title | 会议室标题   |string  |    |
|classTitle | 分区   |string  |    |
|seatNum | 座位数   |integer(int32)  |    |
|times | 占用数据   |array  | RoomUsedTimeDTO   |

**RoomUsedEffectiveDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|clock |    |integer(int32)  |    |
|effectiveEnum | 可用值:USED,NOT_USED,EXPIRED   |string  |    |
|end |    |integer(int32)  |    |
|start |    |integer(int32)  |    |

**RoomUsedTimeDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|statusEnum | 状态,可用值:UNSUBMITTED,DELETED,APPLYING,ENABLED,ENDED,REFUSED,CANCELED,CLOSED   |string  |    |
|meeting | 会议名称   |string  |    |
|startTime | 开始时间   |string  |    |
|startStamp | 开始时间戳   |integer(int32)  |    |
|endTime | 结束时间   |string  |    |
|endStamp | 结束时间戳   |integer(int32)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«RoomUsedDetailDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# room_class

## 新建

**接口描述**:会议室分区

**接口地址**:`/api/room_class/add`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"remark": "备注",
	"title": "关莎楼"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |RoomClassAddParam  | RoomClassAddParam   |

**schema属性说明**



**RoomClassAddParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|remark| 备注  | body | false |string  |    |
|title| 分区名称  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 所有分区

**接口描述**:会议室分区,设置会议

**接口地址**:`/api/room_class/all`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"gmtCreate": "",
			"gmtModify": "",
			"id": 1,
			"remark": "备注",
			"status": "",
			"statusEnum": "",
			"title": "关莎楼"
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | RoomClassAllDTO   |
|message|   |string  |    |



**schema属性说明**




**RoomClassAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|gmtCreate | 创建时间   |string(date-time)  |    |
|gmtModify | 最后编辑   |string(date-time)  |    |
|id | 分区id   |integer(int64)  |    |
|remark | 备注   |string  |    |
|status |    |string  |    |
|statusEnum | 可用值:ENABLED,DISABLED,DELETED   |string  |    |
|title | 分区名称   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«RoomClassAllDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 已启用的分区

**接口描述**:会议室

**接口地址**:`/api/room_class/enableds`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"gmtCreate": "",
			"gmtModify": "",
			"id": 1,
			"remark": "备注",
			"status": "",
			"statusEnum": "",
			"title": "关莎楼"
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | RoomClassAllDTO   |
|message|   |string  |    |



**schema属性说明**




**RoomClassAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|gmtCreate | 创建时间   |string(date-time)  |    |
|gmtModify | 最后编辑   |string(date-time)  |    |
|id | 分区id   |integer(int64)  |    |
|remark | 备注   |string  |    |
|status |    |string  |    |
|statusEnum | 可用值:ENABLED,DISABLED,DELETED   |string  |    |
|title | 分区名称   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«RoomClassAllDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 启用/禁用/删除

**接口描述**:会议室分区

**接口地址**:`/api/room_class/modify_status`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1,
	"status": "ENABLED"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |CommonStatusParam  | CommonStatusParam   |

**schema属性说明**



**CommonStatusParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |
|status| 状态( ENABLED(1, "已启用"), DISABLED(0, "已禁用"), DELETED(-1, "已删除");),可用值:ENABLED,DISABLED,DELETED  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 更新

**接口描述**:会议室分区

**接口地址**:`/api/room_class/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1,
	"remark": "备注",
	"title": "关莎楼"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |RoomClassUpdateParam  | RoomClassUpdateParam   |

**schema属性说明**



**RoomClassUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 分区id  | body | false |integer(int64)  |    |
|remark| 备注  | body | false |string  |    |
|title| 分区名称  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# srs

## 设置推流状态已停止


**接口描述**:


**接口地址**:`/api/srs/set_unpublish`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"groupId": ""
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SrsSetUnpublishParam  | SrsSetUnpublishParam   |

**schema属性说明**



**SrsSetUnpublishParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|groupId|   | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取分组（groupId）是否有人正在推流

**接口描述**:客户端

**接口地址**:`/api/srs/status`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"groupId": "group_mid_1"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SrsStatusParam  | SrsStatusParam   |

**schema属性说明**



**SrsStatusParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|groupId| 分组id  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"isRtmpPushing": false,
		"key": ""
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |SrsStatusDTO  | SrsStatusDTO   |
|message|   |string  |    |



**schema属性说明**




**SrsStatusDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|isRtmpPushing | 是否有人在同屏   |boolean  |    |
|key | 口令   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«SrsStatusDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 测试code


**接口描述**:


**接口地址**:`/api/srs/test_code`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"name": "1cac913bb3a28ad30a7b94e4b2aa8ffb47a1a7d4400c81dea29790b0061f2094"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SrsOnPublishParam  | SrsOnPublishParam   |

**schema属性说明**



**SrsOnPublishParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|name| name  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# summary

## 上传纪要文件

**接口描述**:会议查询,客户端

**接口地址**:`/api/summary/add`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |
|mid| mid  | query | false |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 统一打开纪要文件

**接口描述**:客户端

**接口地址**:`/api/summary/all_open`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"muid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SummaryAllOpenParam  | SummaryAllOpenParam   |

**schema属性说明**



**SummaryAllOpenParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|muid| 会议人员id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 下载文件


**接口描述**:


**接口地址**:`/api/summary/download/{summaryId}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["application/octet-stream"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|summaryId| summaryId  | path | true |integer  |    |

**响应示例**:

```json

```

**响应参数**:


暂无





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会中纪要编辑路径

**接口描述**:客户端

**接口地址**:`/api/summary/edit_url`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"muid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MUIDParam  | MUIDParam   |

**schema属性说明**



**MUIDParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|muid| 会议人员id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议纪要列表

**接口描述**:会议查询,客户端

**接口地址**:`/api/summary/list`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingIdParam  | MeetingIdParam   |

**schema属性说明**



**MeetingIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"summaryId": 0,
			"title": "",
			"url": "",
			"view": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | SummaryListDTO   |
|message|   |string  |    |



**schema属性说明**




**SummaryListDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|summaryId | 纪要id   |integer(int64)  |    |
|title | 文档名称   |string  |    |
|url | 下载路径   |string  |    |
|view | 查看页面   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«SummaryListDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 启用/禁用/删除

**接口描述**:会议查询,文件归档,客户端

**接口地址**:`/api/summary/modify_status`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"status": "ENABLED",
	"summaryId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SummaryModifyStatusParam  | SummaryModifyStatusParam   |

**schema属性说明**



**SummaryModifyStatusParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|status| 状态( ENABLED(1, "已启用"), DISABLED(0, "已禁用"), DELETED(-1, "已删除");),可用值:ENABLED,DISABLED,DELETED  | body | false |string  |    |
|summaryId| 纪要id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会中纪要查看路径

**接口描述**:会议查询,客户端

**接口地址**:`/api/summary/view_url`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"muid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MUIDParam  | MUIDParam   |

**schema属性说明**



**MUIDParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|muid| 会议人员id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# speech

## 打开/关闭语音总结

**接口描述**:客户端

**接口地址**:`/api/speech/cache_on`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"cacheOn": true,
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SpeechCacheOnParam  | SpeechCacheOnParam   |

**schema属性说明**



**SpeechCacheOnParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|cacheOn|   | body | false |boolean  |    |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取识别结果

**接口描述**:客户端

**接口地址**:`/api/speech/device_content`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"muid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SpeechDeviceContentParam  | SpeechDeviceContentParam   |

**schema属性说明**



**SpeechDeviceContentParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|muid| 会议人员id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"content": ""
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |SpeechContentDTO  | SpeechContentDTO   |
|message|   |string  |    |



**schema属性说明**




**SpeechContentDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|content |    |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«SpeechContentDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 识别服务是否已启动

**接口描述**:客户端

**接口地址**:`/api/speech/is_opened`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SpeechIsOpenedParam  | SpeechIsOpenedParam   |

**schema属性说明**



**SpeechIsOpenedParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 语音总结是否打开

**接口描述**:客户端

**接口地址**:`/api/speech/is_speeching`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingIdParam  | MeetingIdParam   |

**schema属性说明**



**MeetingIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 启动识别服务

**接口描述**:客户端

**接口地址**:`/api/speech/start`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SpeechStartParam  | SpeechStartParam   |

**schema属性说明**



**SpeechStartParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议Id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 关闭识别服务

**接口描述**:客户端

**接口地址**:`/api/speech/stop`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SpeechStopParam  | SpeechStopParam   |

**schema属性说明**



**SpeechStopParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议Id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 语音识别设备同步(无需关注：设备自动调用)


**接口描述**:


**接口地址**:`/api/speech/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"deviceCode": "sdxzcasd23ddasd",
	"host": "192.168.0.11:8081"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SpeechUpdateParam  | SpeechUpdateParam   |

**schema属性说明**



**SpeechUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|deviceCode| 语音识别设备码  | body | false |string  |    |
|host| 语音识别设备地址  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# speechs

## 语音数据列表


**接口描述**:


**接口地址**:`/api/speechs/all`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SpeechsAllParam  | SpeechsAllParam   |

**schema属性说明**



**SpeechsAllParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议人员id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"muid": 0,
			"name": "",
			"res": "",
			"time": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | SpeechsAllDTO   |
|message|   |string  |    |



**schema属性说明**




**SpeechsAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|muid |    |integer(int64)  |    |
|name |    |string  |    |
|res |    |string  |    |
|time |    |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«SpeechsAllDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议语音数据全文


**接口描述**:


**接口地址**:`/api/speechs/meeting`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |MeetingIdParam  | MeetingIdParam   |

**schema属性说明**



**MeetingIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 提交语音数据


**接口描述**:


**接口地址**:`/api/speechs/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"content": "内容",
	"muid": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SpeechsUpdateParam  | SpeechsUpdateParam   |

**schema属性说明**



**SpeechsUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|content| 内容  | body | false |string  |    |
|muid| 会议人员id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# setting

## 后台logo配置


**接口描述**:


**接口地址**:`/api/setting/build_backend_logo`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | false |file  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 前台技术支持配置


**接口描述**:


**接口地址**:`/api/setting/build_frontend_company`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|commpany| commpany  | query | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 前台logo配置


**接口描述**:


**接口地址**:`/api/setting/build_frontend_logo`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | false |file  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 前台系统标题图片配置


**接口描述**:


**接口地址**:`/api/setting/build_frontend_sys_img`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | false |file  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## office服务器配置


**接口描述**:


**接口地址**:`/api/setting/build_office_server`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|office_server| office_server  | query | true |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## Word2pdf服务器配置


**接口描述**:


**接口地址**:`/api/setting/build_word2pdf`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|word2pdfIp| word2pdfIp  | query | true |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 客户端获取设置信息，无需token


**接口描述**:


**接口地址**:`/api/setting/device`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"company": "",
		"frontendLogo": "",
		"frontendSysImg": "",
		"logo": "",
		"typeEnum": ""
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |SettingDeviceDTO  | SettingDeviceDTO   |
|message|   |string  |    |



**schema属性说明**




**SettingDeviceDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|company | 保障单位   |string  |    |
|frontendLogo | 前台logo   |string  |    |
|frontendSysImg | 系统标题图片   |string  |    |
|logo | 后台logo   |string  |    |
|typeEnum | 会议类型：     NO_LOGIN(1, "会前安排"),     LOGIN(2, "帐密登录"),     PASSWORD_ONLY(3, "会议密码");,可用值:NO_LOGIN,LOGIN,PASSWORD_ONLY   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«SettingDeviceDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取会议编辑页可用项


**接口描述**:


**接口地址**:`/api/setting/get_meeting_edit_items`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"isHaveTitleEdit": false,
		"isHaveExcelAddMeetingUser": false,
		"isHaveMeetingType": false,
		"isHaveGoods": false,
		"isHaveIssue": false,
		"isHaveVote": false,
		"isHaveVideo": false,
		"isHaveAgenda": false,
		"isHaveFrontScreen": false,
		"isHaveBackScreen": false
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |SettingMeetingEditItemsParam  | SettingMeetingEditItemsParam   |
|message|   |string  |    |



**schema属性说明**




**SettingMeetingEditItemsParam**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|isHaveTitleEdit | 是否有单独编辑会议标题   |boolean  |    |
|isHaveExcelAddMeetingUser | 是否有临时人员Excel导入   |boolean  |    |
|isHaveMeetingType | 是否有会议类型   |boolean  |    |
|isHaveGoods | 是否有会议用品   |boolean  |    |
|isHaveIssue | 是否有议题材料   |boolean  |    |
|isHaveVote | 是否有投票   |boolean  |    |
|isHaveVideo | 是否有视频   |boolean  |    |
|isHaveAgenda | 是否有议程   |boolean  |    |
|isHaveFrontScreen | 是否有前屏设置   |boolean  |    |
|isHaveBackScreen | 是否有后屏设置   |boolean  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«SettingMeetingEditItemsParam»|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取可选配置信息


**接口描述**:


**接口地址**:`/api/setting/get_values`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"keys": []
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SettingKeysParam  | SettingKeysParam   |

**schema属性说明**



**SettingKeysParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|keys| 关键字数组，参数范围 ["hasReview","meetingType", "meetingStatus"]  | body | false |array  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«HashMap«string,object»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 设置是否有审核


**接口描述**:


**接口地址**:`/api/setting/has_review`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"hasReview": false
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SettingHasReviewParam  | SettingHasReviewParam   |

**schema属性说明**



**SettingHasReviewParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|hasReview| 是否有审核  | body | false |boolean  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取是否可强制同屏-顶掉前个投屏


**接口描述**:


**接口地址**:`/api/setting/live_power`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 设置是否可强制同屏-顶掉前个投屏


**接口描述**:


**接口地址**:`/api/setting/live_power`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"isLivePower": false
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SettingLivePowerParam  | SettingLivePowerParam   |

**schema属性说明**



**SettingLivePowerParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|isLivePower| 是否能强制投屏  | body | true |boolean  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 设置会议编辑页可用项


**接口描述**:


**接口地址**:`/api/setting/meeting_edit_items`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"isHaveTitleEdit": false,
	"isHaveExcelAddMeetingUser": false,
	"isHaveMeetingType": false,
	"isHaveGoods": false,
	"isHaveIssue": false,
	"isHaveVote": false,
	"isHaveVideo": false,
	"isHaveAgenda": false,
	"isHaveFrontScreen": false,
	"isHaveBackScreen": false
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SettingMeetingEditItemsParam  | SettingMeetingEditItemsParam   |

**schema属性说明**



**SettingMeetingEditItemsParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|isHaveTitleEdit| 是否有单独编辑会议标题  | body | false |boolean  |    |
|isHaveExcelAddMeetingUser| 是否有临时人员Excel导入  | body | false |boolean  |    |
|isHaveMeetingType| 是否有会议类型  | body | false |boolean  |    |
|isHaveGoods| 是否有会议用品  | body | false |boolean  |    |
|isHaveIssue| 是否有议题材料  | body | false |boolean  |    |
|isHaveVote| 是否有投票  | body | false |boolean  |    |
|isHaveVideo| 是否有视频  | body | false |boolean  |    |
|isHaveAgenda| 是否有议程  | body | false |boolean  |    |
|isHaveFrontScreen| 是否有前屏设置  | body | false |boolean  |    |
|isHaveBackScreen| 是否有后屏设置  | body | false |boolean  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议模式列表

**接口描述**:会议模式

**接口地址**:`/api/setting/meeting_types`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"isSelected": false,
			"meetingType": "",
			"meetingTypeEnum": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | MeetingTypesDTO   |
|message|   |string  |    |



**schema属性说明**




**MeetingTypesDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|isSelected | 是否选中   |boolean  |    |
|meetingType | 会议模式   |string  |    |
|meetingTypeEnum | 会议模式,可用值:NO_LOGIN,LOGIN,PASSWORD_ONLY   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«MeetingTypesDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 是否桌牌后台（获取）


**接口描述**:


**接口地址**:`/api/setting/only_deskcard`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 删除redis某个值


**接口描述**:


**接口地址**:`/api/setting/redis`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"key": "",
	"second": 0
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SettingRedisParam  | SettingRedisParam   |

**schema属性说明**



**SettingRedisParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|key|   | body | false |string  |    |
|second|   | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 查询redis某个值


**接口描述**:


**接口地址**:`/api/setting/redis_find`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"key": ""
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SettingRedisFindParam  | SettingRedisFindParam   |

**schema属性说明**



**SettingRedisFindParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|key|   | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 设置当前桌牌模板

**接口描述**:桌牌模板

**接口地址**:`/api/setting/set_desk_card`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 设置当前首页模板

**接口描述**:首页模板

**接口地址**:`/api/setting/set_home_page`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 设置ip


**接口描述**:


**接口地址**:`/api/setting/set_ip`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"dns1": "192.168.0.1",
	"gateway": "192.168.0.1",
	"ip": "192.168.0.241"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SettingSetIpParam  | SettingSetIpParam   |

**schema属性说明**



**SettingSetIpParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|dns1| DNS1  | body | false |string  |    |
|gateway| gateway  | body | false |string  |    |
|ip| ip地址  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 设置会议模式

**接口描述**:会议模式

**接口地址**:`/api/setting/set_meeting_type`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"typeEnum": ""
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SettingSetMeetingTypeParam  | SettingSetMeetingTypeParam   |

**schema属性说明**



**SettingSetMeetingTypeParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|typeEnum| 会议模式标识,    NO_LOGIN(1, "会前安排"),<br/>    LOGIN(2, "帐密登录"),<br/>    PASSWORD_ONLY(3, "会议密码"),可用值:NO_LOGIN,LOGIN,PASSWORD_ONLY  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 是否桌牌后台（设置）


**接口描述**:


**接口地址**:`/api/setting/set_only_deskcard`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"onlyDeskcard": false
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SettingSetOnlyDeskcardParam  | SettingSetOnlyDeskcardParam   |

**schema属性说明**



**SettingSetOnlyDeskcardParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|onlyDeskcard| 会议编辑只有刷桌牌  | body | false |boolean  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 设置系统时间


**接口描述**:


**接口地址**:`/api/setting/set_system_time`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"time": "2020-01-23 09:00:00"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SettingSetSystemTimeParam  | SettingSetSystemTimeParam   |

**schema属性说明**



**SettingSetSystemTimeParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|time| 系统时间  | body | false |string(date-time)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# summary_sign

## 下载签字列表word


**接口描述**:


**接口地址**:`/api/summary_sign/download/{summaryId}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["application/octet-stream"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|summaryId| summaryId  | path | true |integer  |    |

**响应示例**:

```json

```

**响应参数**:


暂无





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 下载纪要-合成签字列表word


**接口描述**:


**接口地址**:`/api/summary_sign/download_word_plus_sign/{summaryId}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["application/octet-stream"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|summaryId| summaryId  | path | true |integer  |    |

**响应示例**:

```json

```

**响应参数**:


暂无





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 是否已签字

**接口描述**:客户端

**接口地址**:`/api/summary_sign/is_signed`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"summaryId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SummarySignIsSignedParam  | SummarySignIsSignedParam   |

**schema属性说明**



**SummarySignIsSignedParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|summaryId| 纪要id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 签字列表

**接口描述**:客户端,会议查询

**接口地址**:`/api/summary_sign/list`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"summaryId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SummarySignListParam  | SummarySignListParam   |

**schema属性说明**



**SummarySignListParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|summaryId| 纪要id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"gmtModify": "",
			"name": "",
			"summarySignId": 0,
			"url": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | SummarySignListDTO   |
|message|   |string  |    |



**schema属性说明**




**SummarySignListDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|gmtModify | 签字时间   |string(date-time)  |    |
|name | 名字   |string  |    |
|summarySignId | 签字id   |integer(int64)  |    |
|url | 签字图片   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«SummarySignListDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 纪要签字

**接口描述**:客户端

**接口地址**:`/api/summary_sign/sign`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |
|summaryId| summaryId  | query | true |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 签字排序

**接口描述**:客户端

**接口地址**:`/api/summary_sign/sort`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"ids": [],
	"summaryId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |SummarySignSortParam  | SummarySignSortParam   |

**schema属性说明**



**SummarySignSortParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|ids| 签字id数组  | body | false |array  |    |
|summaryId| 纪要id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# token

## 服务器授权


**接口描述**:


**接口地址**:`/api/token/authorization`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"authorization": ""
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |TokenAuthorizationParam  | TokenAuthorizationParam   |

**schema属性说明**



**TokenAuthorizationParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|authorization| 授权码  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 客户端扫码选人登录


**接口描述**:


**接口地址**:`/api/token/client_scan_token`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"account": "user",
	"muid": 1,
	"password": "123"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |TokenClientScanTokenParam  | TokenClientScanTokenParam   |

**schema属性说明**



**TokenClientScanTokenParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|account| 账户名（有帐密就验证，无直接生成token）  | body | false |string  |    |
|muid| 与会人员id  | body | false |integer(int64)  |    |
|password| 密码（有帐密就验证，无直接生成token）  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"name": "",
		"room": "",
		"time": "",
		"title": "",
		"token": ""
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |TokenInitsDTO  | TokenInitsDTO   |
|message|   |string  |    |



**schema属性说明**




**TokenInitsDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|name | 姓名   |string  |    |
|room | 会议室   |string  |    |
|time | 会议时间   |string  |    |
|title | 会议标题   |string  |    |
|token | 令牌   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«TokenInitsDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 第三方登录页返回code获取_uid查找user返回token（后台网页用）


**接口描述**:


**接口地址**:`/api/token/code2token`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|code| code  | query | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"name": "",
		"room": "",
		"time": "",
		"title": "",
		"token": ""
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |TokenInitsDTO  | TokenInitsDTO   |
|message|   |string  |    |



**schema属性说明**




**TokenInitsDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|name | 姓名   |string  |    |
|room | 会议室   |string  |    |
|time | 会议时间   |string  |    |
|title | 会议标题   |string  |    |
|token | 令牌   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«TokenInitsDTO»|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 获取token数组


**接口描述**:


**接口地址**:`/api/token/inits`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"account": "user",
	"deviceCode": "deviceCode",
	"deviceType": "WEB",
	"faceImgPath": "uploads/20191024/d763b2d6-74c0-46ee-8068-97236ecff2cc.png",
	"password": "123"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |TokenInitParam  | TokenInitParam   |

**schema属性说明**



**TokenInitParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|account| 账户名  | body | false |string  |    |
|deviceCode| 设备唯一标识码：web为空字符串  | body | false |string  |    |
|deviceType| 设备类型<br/>    CLIENT_DESKCARD(11, "客户端自带桌牌"),<br/>    CLIENT_ONLY(12, "单独客户端"),<br/>    DESKCARD_ONLY(13, "单独桌牌"),<br/>    CLIENT_DESKCARD_WITHOUT_DESKCARD(14, "客户端（外置桌牌）"),<br/>    CLIENT_DESKCARD_WITHOUT_CLIENT(15, "外置桌牌"),<br/>    MOBILE_CLIENT(16, "移动客户端"),<br/>    BIG_SCREEN(17, "大屏"),<br/>    ROOMCARD(21, "门牌"),<br/>    WEB(31, "web");,可用值:CLIENT_DESKCARD,CLIENT_ONLY,DESKCARD_ONLY,CLIENT_DESKCARD_WITHOUT_DESKCARD,CLIENT_DESKCARD_WITHOUT_CLIENT,MOBILE_CLIENT,BIG_SCREEN,ROOMCARD,WEB  | body | false |string  |    |
|faceImgPath| 人脸图片(非人脸识别登录填'空字符串'，需上传图片并获取路径)  | body | false |string  |    |
|password| 密码  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"name": "",
			"room": "",
			"time": "",
			"title": "",
			"token": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | TokenInitsDTO   |
|message|   |string  |    |



**schema属性说明**




**TokenInitsDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|name | 姓名   |string  |    |
|room | 会议室   |string  |    |
|time | 会议时间   |string  |    |
|title | 会议标题   |string  |    |
|token | 令牌   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«TokenInitsDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 手机扫码（帐密）登录（投票用）


**接口描述**:


**接口地址**:`/api/token/mobile_scan_token`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"account": "user",
	"password": "123",
	"voteId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |TokenMobileScanTokenParam  | TokenMobileScanTokenParam   |

**schema属性说明**



**TokenMobileScanTokenParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|account| 账户名  | body | false |string  |    |
|password| 密码  | body | false |string  |    |
|voteId| 投票id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"name": "",
		"room": "",
		"time": "",
		"title": "",
		"token": ""
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |TokenInitsDTO  | TokenInitsDTO   |
|message|   |string  |    |



**schema属性说明**




**TokenInitsDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|name | 姓名   |string  |    |
|room | 会议室   |string  |    |
|time | 会议时间   |string  |    |
|title | 会议标题   |string  |    |
|token | 令牌   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«TokenInitsDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 政务微信登录返回MUID-token（移动端用）


**接口描述**:


**接口地址**:`/api/token/openid_mid_token`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1,
	"openId": "5e4109f475c1bd50781d6124"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |TokenOpenIdMidParam  | TokenOpenIdMidParam   |

**schema属性说明**



**TokenOpenIdMidParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |
|openId| 微信openId  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"name": "",
		"room": "",
		"time": "",
		"title": "",
		"token": ""
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |TokenInitsDTO  | TokenInitsDTO   |
|message|   |string  |    |



**schema属性说明**




**TokenInitsDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|name | 姓名   |string  |    |
|room | 会议室   |string  |    |
|time | 会议时间   |string  |    |
|title | 会议标题   |string  |    |
|token | 令牌   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«TokenInitsDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 政务微信js返回openId获取UID-token（移动端审核后台用）


**接口描述**:


**接口地址**:`/api/token/openid_token`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"openId": "5e4109f475c1bd50781d6124"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |TokenOpenIdParam  | TokenOpenIdParam   |

**schema属性说明**



**TokenOpenIdParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|openId| 微信openId  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"name": "",
		"room": "",
		"time": "",
		"title": "",
		"token": ""
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |TokenInitsDTO  | TokenInitsDTO   |
|message|   |string  |    |



**schema属性说明**




**TokenInitsDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|name | 姓名   |string  |    |
|room | 会议室   |string  |    |
|time | 会议时间   |string  |    |
|title | 会议标题   |string  |    |
|token | 令牌   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«TokenInitsDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 上传人脸


**接口描述**:


**接口地址**:`/api/token/upload`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 政务微信扫码登录地址(后台授权登录)


**接口描述**:


**接口地址**:`/api/token/wechat_scan_url`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 政务微信扫码投票获取token


**接口描述**:


**接口地址**:`/api/token/wechat_token`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"openId": "5e4109f475c1bd50781d6124",
	"voteId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |TokenWechatParam  | TokenWechatParam   |

**schema属性说明**



**TokenWechatParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|openId| 微信openId  | body | false |string  |    |
|voteId| 投票id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"name": "",
		"room": "",
		"time": "",
		"title": "",
		"token": ""
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |TokenInitsDTO  | TokenInitsDTO   |
|message|   |string  |    |



**schema属性说明**




**TokenInitsDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|name | 姓名   |string  |    |
|room | 会议室   |string  |    |
|time | 会议时间   |string  |    |
|title | 会议标题   |string  |    |
|token | 令牌   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«TokenInitsDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# user

## 新增

**接口描述**:人员列表

**接口地址**:`/api/user/add`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"account": "user001",
	"departmentId": 1,
	"email": "emailname@163.com",
	"mobile": "13611112222",
	"name": "王大锤",
	"password": "123",
	"post": "普通员工",
	"role": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |UserAddParam  | UserAddParam   |

**schema属性说明**



**UserAddParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|account| 账号  | body | false |string  |    |
|departmentId| 部门  | body | false |integer(int64)  |    |
|email| 邮箱  | body | false |string  |    |
|mobile| 手机  | body | false |string  |    |
|name| 姓名  | body | false |string  |    |
|password| 密码  | body | false |string  |    |
|post| 职位  | body | false |string  |    |
|role| 角色  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 新增服务人员


**接口描述**:


**接口地址**:`/api/user/add_server`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"uids": []
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |UserAddServerParam  | UserAddServerParam   |

**schema属性说明**



**UserAddServerParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|uids| 用户id数组  | body | false |array  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 所有人员

**接口描述**:人员列表

**接口地址**:`/api/user/all`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"account": "",
			"department": "",
			"gmtCreate": "",
			"gmtModify": "",
			"id": 0,
			"name": "",
			"post": "",
			"remark": "",
			"role": "",
			"status": "",
			"statusEnum": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | UserAllDTO   |
|message|   |string  |    |



**schema属性说明**




**UserAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|account | 账户   |string  |    |
|department | 部门   |string  |    |
|gmtCreate | 创建时间   |string(date-time)  |    |
|gmtModify | 最后修改   |string(date-time)  |    |
|id | 用户id   |integer(int64)  |    |
|name | 用户名称   |string  |    |
|post | 职位   |string  |    |
|remark | 备注   |string  |    |
|role | 角色   |string  |    |
|status | 状态，已删除，已禁用，已启用   |string  |    |
|statusEnum | 状态，已删除，已禁用，已启用,可用值:ENABLED,DISABLED,DELETED   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«UserAllDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 所有部门人员

**接口描述**:人员列表,我的申请

**接口地址**:`/api/user/all_department_users`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"department": "",
			"users": [
				{
					"id": 0,
					"name": "",
					"post": "",
					"remark": "",
					"role": ""
				}
			]
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | UserAllDepartmentUsersDTO   |
|message|   |string  |    |



**schema属性说明**




**UserAllDepartmentUsersDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|department | 部门   |string  |    |
|users | 用户list   |array  | UserAllDepartmentUsersSubDTO   |

**UserAllDepartmentUsersSubDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|id | 用户id   |integer(int64)  |    |
|name | 用户名称   |string  |    |
|post | 职位   |string  |    |
|remark | 备注   |string  |    |
|role | 角色   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«UserAllDepartmentUsersDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 所有服务人员


**接口描述**:


**接口地址**:`/api/user/allservers`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"department": "",
			"departmentId": 0,
			"isSelected": false,
			"name": "",
			"uid": 0
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | UserServersDTO   |
|message|   |string  |    |



**schema属性说明**




**UserServersDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|department | 部门   |string  |    |
|departmentId | 部门id   |integer(int64)  |    |
|isSelected | 是否选中   |boolean  |    |
|name | 人员姓名   |string  |    |
|uid | 人员id   |integer(int64)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«UserServersDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 删除服务人员


**接口描述**:


**接口地址**:`/api/user/delete_server`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"uids": []
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |UserAddServerParam  | UserAddServerParam   |

**schema属性说明**



**UserAddServerParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|uids| 用户id数组  | body | false |array  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 部门人员

**接口描述**:人员列表,我的申请

**接口地址**:`/api/user/department`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1,
	"name": "张三",
	"isReview": false
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |UserDepartmentParam  | UserDepartmentParam   |

**schema属性说明**



**UserDepartmentParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 部门id，departmentId  | body | true |integer(int64)  |    |
|name| 姓名  | body | false |string  |    |
|isReview| 是否只返回有审核权限的人员  | body | false |boolean  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"account": "",
			"department": "",
			"gmtCreate": "",
			"gmtModify": "",
			"id": 0,
			"name": "",
			"post": "",
			"remark": "",
			"role": "",
			"status": "",
			"statusEnum": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | UserAllDTO   |
|message|   |string  |    |



**schema属性说明**




**UserAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|account | 账户   |string  |    |
|department | 部门   |string  |    |
|gmtCreate | 创建时间   |string(date-time)  |    |
|gmtModify | 最后修改   |string(date-time)  |    |
|id | 用户id   |integer(int64)  |    |
|name | 用户名称   |string  |    |
|post | 职位   |string  |    |
|remark | 备注   |string  |    |
|role | 角色   |string  |    |
|status | 状态，已删除，已禁用，已启用   |string  |    |
|statusEnum | 状态，已删除，已禁用，已启用,可用值:ENABLED,DISABLED,DELETED   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«UserAllDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 人员详情

**接口描述**:人员列表

**接口地址**:`/api/user/detail`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"account": "",
		"departmentId": 0,
		"email": "",
		"id": 0,
		"mobile": "",
		"name": "",
		"post": "",
		"remark": "",
		"role": 0,
		"status": 0
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |UserDetailDTO  | UserDetailDTO   |
|message|   |string  |    |



**schema属性说明**




**UserDetailDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|account |    |string  |    |
|departmentId |    |integer(int64)  |    |
|email |    |string  |    |
|id |    |integer(int64)  |    |
|mobile |    |string  |    |
|name |    |string  |    |
|post |    |string  |    |
|remark |    |string  |    |
|role |    |integer(int64)  |    |
|status |    |integer(int32)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«UserDetailDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 回收站清空单个人员

**接口描述**:人员回收站

**接口地址**:`/api/user/empty`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 人脸录入

**接口描述**:人员列表

**接口地址**:`/api/user/face_encoding`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |
|uid| uid  | query | false |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 启用/禁用/删除

**接口描述**:人员列表

**接口地址**:`/api/user/modify_status`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1,
	"status": "ENABLED"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |CommonStatusParam  | CommonStatusParam   |

**schema属性说明**



**CommonStatusParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |
|status| 状态( ENABLED(1, "已启用"), DISABLED(0, "已禁用"), DELETED(-1, "已删除");),可用值:ENABLED,DISABLED,DELETED  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 回收站

**接口描述**:人员回收站

**接口地址**:`/api/user/recycle`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"account": "",
			"department": "",
			"gmtCreate": "",
			"gmtModify": "",
			"id": 0,
			"name": "",
			"post": "",
			"remark": "",
			"role": "",
			"status": "",
			"statusEnum": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | UserAllDTO   |
|message|   |string  |    |



**schema属性说明**




**UserAllDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|account | 账户   |string  |    |
|department | 部门   |string  |    |
|gmtCreate | 创建时间   |string(date-time)  |    |
|gmtModify | 最后修改   |string(date-time)  |    |
|id | 用户id   |integer(int64)  |    |
|name | 用户名称   |string  |    |
|post | 职位   |string  |    |
|remark | 备注   |string  |    |
|role | 角色   |string  |    |
|status | 状态，已删除，已禁用，已启用   |string  |    |
|statusEnum | 状态，已删除，已禁用，已启用,可用值:ENABLED,DISABLED,DELETED   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«UserAllDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 同步第三方部门人员


**接口描述**:


**接口地址**:`/api/user/reload_users`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 服务人员（包括未添加）


**接口描述**:


**接口地址**:`/api/user/servers`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"departmentId": 0
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |UserServersParam  | UserServersParam   |

**schema属性说明**



**UserServersParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|departmentId| 部门id,全部门为0  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"department": "",
			"departmentId": 0,
			"isSelected": false,
			"name": "",
			"uid": 0
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | UserServersDTO   |
|message|   |string  |    |



**schema属性说明**




**UserServersDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|department | 部门   |string  |    |
|departmentId | 部门id   |integer(int64)  |    |
|isSelected | 是否选中   |boolean  |    |
|name | 人员姓名   |string  |    |
|uid | 人员id   |integer(int64)  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«UserServersDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 更新

**接口描述**:人员列表

**接口地址**:`/api/user/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"departmentId": 1,
	"email": "emailname@163.com",
	"id": 1,
	"mobile": "13611112222",
	"name": "王大锤",
	"password": "123",
	"post": "普通员工",
	"role": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |UserUpdateParam  | UserUpdateParam   |

**schema属性说明**



**UserUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|departmentId| 部门  | body | false |integer(int64)  |    |
|email| 邮箱  | body | false |string  |    |
|id| 用户id  | body | false |integer(int64)  |    |
|mobile| 手机  | body | false |string  |    |
|name| 姓名  | body | false |string  |    |
|password| 密码  | body | false |string  |    |
|post| 职位  | body | false |string  |    |
|role| 角色  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# video

## 添加会议视频（上传视频文件）

**接口描述**:我的申请

**接口地址**:`/api/video/add_file`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |
|mid| mid  | query | false |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 添加会议室视频（上传视频文件）

**接口描述**:会议室

**接口地址**:`/api/video/add_room_file`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |
|roomId| roomId  | query | false |integer  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 添加会议室视频（上传视频链接）

**接口描述**:会议室

**接口地址**:`/api/video/add_room_url`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"name": "外链名称",
	"remark": "备注",
	"roomId": 1,
	"url": "https://www.baidu.com/"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |VideoAddRoomUrlParam  | VideoAddRoomUrlParam   |

**schema属性说明**



**VideoAddRoomUrlParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|name| 视频名称  | body | false |string  |    |
|remark| 备注  | body | false |string  |    |
|roomId| 会议室id  | body | false |integer(int64)  |    |
|url| 视频url  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 添加会议视频（上传视频链接）

**接口描述**:我的申请

**接口地址**:`/api/video/add_url`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"mid": 1,
	"name": "外链名称",
	"remark": "备注",
	"url": "https://www.baidu.com/"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |VideoAddUrlParam  | VideoAddUrlParam   |

**schema属性说明**



**VideoAddUrlParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|mid| 会议id  | body | false |integer(int64)  |    |
|name| 视频名称  | body | false |string  |    |
|remark| 备注  | body | false |string  |    |
|url| 视频url  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议视频列表

**接口描述**:我的申请,客户端

**接口地址**:`/api/video/meeting`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"gmtModify": "",
			"id": 0,
			"isUrl": false,
			"name": "",
			"remark": "",
			"screenShot": "",
			"size": "",
			"uploadUser": "",
			"url": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | VideoMeetingDTO   |
|message|   |string  |    |



**schema属性说明**




**VideoMeetingDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|gmtModify | 时间   |string(date-time)  |    |
|id | 视频id   |integer(int64)  |    |
|isUrl | 是否外链   |boolean  |    |
|name | 名称   |string  |    |
|remark | 备注   |string  |    |
|screenShot | 缩略图   |string  |    |
|size | 大小   |string  |    |
|uploadUser | 上传人   |string  |    |
|url | url   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«VideoMeetingDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 启用/禁用/删除

**接口描述**:我的申请

**接口地址**:`/api/video/modify_status`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1,
	"status": "ENABLED"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |CommonStatusParam  | CommonStatusParam   |

**schema属性说明**



**CommonStatusParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |
|status| 状态( ENABLED(1, "已启用"), DISABLED(0, "已禁用"), DELETED(-1, "已删除");),可用值:ENABLED,DISABLED,DELETED  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议室视频列表

**接口描述**:会议室,客户端

**接口地址**:`/api/video/room`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"pageNum": 1,
	"pageSize": 10,
	"roomId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |VideoRoomParam  | VideoRoomParam   |

**schema属性说明**



**VideoRoomParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|pageNum| 当前页号  | body | false |integer(int32)  |    |
|pageSize| 每页记录数  | body | false |integer(int32)  |    |
|roomId| 会议室id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"hasNext": true,
		"list": [
			{
				"gmtModify": "",
				"id": 0,
				"isUrl": false,
				"name": "",
				"remark": "",
				"screenShot": "",
				"size": "",
				"status": "",
				"statusEnum": "",
				"uploadUser": "",
				"url": ""
			}
		],
		"pageNum": 0,
		"pageSize": 0,
		"totalCount": 0,
		"totalPage": 0
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |PageDTO«VideoRoomDTO»  | PageDTO«VideoRoomDTO»   |
|message|   |string  |    |



**schema属性说明**




**PageDTO«VideoRoomDTO»**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|hasNext |    |boolean  |    |
|list |    |array  | VideoRoomDTO   |
|pageNum |    |integer(int32)  |    |
|pageSize |    |integer(int32)  |    |
|totalCount |    |integer(int32)  |    |
|totalPage |    |integer(int32)  |    |

**VideoRoomDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|gmtModify | 时间   |string(date-time)  |    |
|id | 视频id   |integer(int64)  |    |
|isUrl | 是否外链   |boolean  |    |
|name | 名称   |string  |    |
|remark | 备注   |string  |    |
|screenShot | 缩略图   |string  |    |
|size | 大小   |string  |    |
|status | 状态   |string  |    |
|statusEnum | 状态标识,可用值:ENABLED,DISABLED,DELETED   |string  |    |
|uploadUser | 上传人   |string  |    |
|url | url   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«PageDTO«VideoRoomDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# vote

## 投票操作

**接口描述**:客户端

**接口地址**:`/api/vote/action`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"groupPre": "group_mid_",
	"options": [],
	"voteId": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |VoteActionParam  | VoteActionParam   |

**schema属性说明**



**VoteActionParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|groupPre| 通讯-组标识前缀  | body | false |string  |    |
|options| 选中的投票选项  | body | false |array  |    |
|voteId| 投票id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 新建

**接口描述**:我的申请,客户端

**接口地址**:`/api/vote/add`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"isAnon": false,
	"isMultiple": false,
	"mid": 1,
	"options": [],
	"remark": "",
	"title": "第一次投票"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |VoteAddParam  | VoteAddParam   |

**schema属性说明**



**VoteAddParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|isAnon| 是否实名  | body | false |boolean  |    |
|isMultiple| 是否多选  | body | false |boolean  |    |
|mid| 会议id  | body | false |integer(int64)  |    |
|options| 投票选项  | body | false |array  |    |
|remark| 备注  | body | false |string  |    |
|title| 投票名称  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 发起/结束投票

**接口描述**:客户端

**接口地址**:`/api/vote/begin_or_end`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"groupPre": "group_mid_",
	"isVoting": false,
	"voteId": 0
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |VoteBeginOrEndParam  | VoteBeginOrEndParam   |

**schema属性说明**



**VoteBeginOrEndParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|groupPre| 通讯-组标识前缀  | body | false |string  |    |
|isVoting| 发起/结束  | body | false |boolean  |    |
|voteId| 投票id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 查看我的投票详情

**接口描述**:客户端

**接口地址**:`/api/vote/detail`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"muid": 0,
	"voteId": 0
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |VoteDetailParam  | VoteDetailParam   |

**schema属性说明**



**VoteDetailParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|muid| 会议人员id  | body | false |integer(int64)  |    |
|voteId| 投票id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"isAnon": false,
		"isMultiple": false,
		"options": [
			{
				"isSelected": false,
				"option": ""
			}
		],
		"remark": "",
		"title": ""
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |VoteDetailDTO  | VoteDetailDTO   |
|message|   |string  |    |



**schema属性说明**




**VoteDetailDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|isAnon | 是否实名   |boolean  |    |
|isMultiple | 是否多选   |boolean  |    |
|options | 选项详情   |array  | VoteDetailSubDTO   |
|remark | 备注   |string  |    |
|title | 投票名称   |string  |    |

**VoteDetailSubDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|isSelected | 是否选中   |boolean  |    |
|option | 选项   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«VoteDetailDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议正在投票列表

**接口描述**:客户端

**接口地址**:`/api/vote/enableds`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"id": 0,
			"isAnon": false,
			"isMultiple": false,
			"isVoting": false,
			"options": [],
			"remark": "",
			"title": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | VoteMeetingDTO   |
|message|   |string  |    |



**schema属性说明**




**VoteMeetingDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|id | 投票id   |integer(int64)  |    |
|isAnon | 是否实名   |boolean  |    |
|isMultiple | 是否多选   |boolean  |    |
|isVoting | 是否投票中   |boolean  |    |
|options | 投票选项   |array  |    |
|remark | 备注   |string  |    |
|title | 投票名称   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«VoteMeetingDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 导出投票结果excel


**接口描述**:


**接口地址**:`/api/vote/get_excel/{voteId}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["application/octet-stream"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|voteId| voteId  | path | true |integer  |    |

**响应示例**:

```json

```

**响应参数**:


暂无





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 会议投票列表

**接口描述**:我的申请,客户端

**接口地址**:`/api/vote/meeting`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": [
		{
			"id": 0,
			"isAnon": false,
			"isMultiple": false,
			"isVoting": false,
			"options": [],
			"remark": "",
			"title": ""
		}
	],
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |array  | VoteMeetingDTO   |
|message|   |string  |    |



**schema属性说明**




**VoteMeetingDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|id | 投票id   |integer(int64)  |    |
|isAnon | 是否实名   |boolean  |    |
|isMultiple | 是否多选   |boolean  |    |
|isVoting | 是否投票中   |boolean  |    |
|options | 投票选项   |array  |    |
|remark | 备注   |string  |    |
|title | 投票名称   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«List«VoteMeetingDTO»»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 删除

**接口描述**:我的申请,客户端

**接口地址**:`/api/vote/modify_status`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1,
	"status": "ENABLED"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |CommonStatusParam  | CommonStatusParam   |

**schema属性说明**



**CommonStatusParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |
|status| 状态( ENABLED(1, "已启用"), DISABLED(0, "已禁用"), DELETED(-1, "已删除");),可用值:ENABLED,DISABLED,DELETED  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 投票二维码（手机扫码投票）


**接口描述**:


**接口地址**:`/api/vote/qrcode/{voteId}`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["application/octet-stream"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|voteId| voteId  | path | true |integer  |    |

**响应示例**:

```json

```

**响应参数**:


暂无





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 投票结果

**接口描述**:会议查询,客户端

**接口地址**:`/api/vote/result`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |PrimaryKeyParam  | PrimaryKeyParam   |

**schema属性说明**



**PrimaryKeyParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 主键id  | body | false |integer(int64)  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {
		"isAnon": "",
		"isMultiple": "",
		"mid": 0,
		"records": [
			{
				"count": 0,
				"option": ""
			}
		],
		"title": "",
		"voteCount": 0
	},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |VoteResultDTO  | VoteResultDTO   |
|message|   |string  |    |



**schema属性说明**




**VoteResultDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|isAnon | 是否实名   |string  |    |
|isMultiple | 是否多选   |string  |    |
|mid | 会议id   |integer(int64)  |    |
|records | 投票详情   |array  | VoteResultSubDTO   |
|title | 投票标题   |string  |    |
|voteCount | 已投票人数   |integer(int32)  |    |

**VoteResultSubDTO**

| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | ------------------|--------|----------- |
|count | 票数   |integer(int32)  |    |
|option | 选项   |string  |    |

**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result«VoteResultDTO»|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 编辑

**接口描述**:我的申请,客户端

**接口地址**:`/api/vote/update`


**请求方式**：`POST`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`


**请求示例**：
```json
{
	"id": 1,
	"isAnon": false,
	"isMultiple": false,
	"options": [],
	"remark": "",
	"title": "第一次投票更新"
}
```


**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|param| param  | body | true |VoteUpdateParam  | VoteUpdateParam   |

**schema属性说明**



**VoteUpdateParam**

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|id| 投票id  | body | false |integer(int64)  |    |
|isAnon| 是否实名  | body | false |boolean  |    |
|isMultiple| 是否多选  | body | false |boolean  |    |
|options| 投票选项  | body | false |array  |    |
|remark| 备注  | body | false |string  |    |
|title| 投票名称  | body | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
# update_task

## 设备在线数


**接口描述**:


**接口地址**:`/api/update_task/device_onlines`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## (框架更新)zip更新文件上传并执行更新


**接口描述**:


**接口地址**:`/api/update_task/update`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## (功能更新)zip更新文件上传并执行更新


**接口描述**:


**接口地址**:`/api/update_task/update_asar`


**请求方式**：`POST`


**consumes**:`["multipart/form-data"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |
|file| file  | formData | true |file  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 201 | Created  ||
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
## 查看更新情况


**接口描述**:


**接口地址**:`/api/update_task/update_status`


**请求方式**：`GET`


**consumes**:`["application/json"]`


**produces**:`["*/*"]`



**请求参数**：

| 参数名称         | 参数说明     |     in |  是否必须      |  数据类型  |  schema  |
| ------------ | -------------------------------- |-----------|--------|----|--- |
|Authorization| 令牌  | header | false |string  |    |

**响应示例**:

```json
{
	"code": 0,
	"data": {},
	"message": ""
}
```

**响应参数**:


| 参数名称         | 参数说明                             |    类型 |  schema |
| ------------ | -------------------|-------|----------- |
|code|   |integer(int32)  | integer(int32)   |
|data|   |object  |    |
|message|   |string  |    |





**响应状态**:


| 状态码         | 说明                            |    schema                         |
| ------------ | -------------------------------- |---------------------- |
| 200 | OK  |Result|
| 401 | Unauthorized  ||
| 403 | Forbidden  ||
| 404 | Not Found  ||
