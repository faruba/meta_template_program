////// Generate by extract.js should not edit by person 
DEFINE_STRUCT_SCHEMA(agip::SAGIPBind,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(str_Gateway_Code, "gatewayCode"),
	DEFINE_STRUCT_FIELD(str_Password, "password"),
	DEFINE_STRUCT_FIELD(str_MAC, "mac"),
	DEFINE_STRUCT_FIELD(c_Reconnect_Flag, "reconnectFlag"),
	DEFINE_STRUCT_FIELD(c_Pad, "pad"),
	DEFINE_STRUCT_FIELD(us_Pad, "us_Pad"),
	DEFINE_STRUCT_FIELD(un_Server_ID, "serverId"),
	DEFINE_STRUCT_FIELD(checkSum, "&crc"));
DEFINE_STRUCT_SCHEMA(agip::SAGIPBindRes,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(n_Result_Code, "resultCode"),
	DEFINE_STRUCT_FIELD(un_Gateway_ID, "gatewayId"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
FUNCTION_BIND(Bind);

DEFINE_STRUCT_SCHEMA(agip::SAGIPCreateRole,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(un_User_ID, "userId"),
	DEFINE_STRUCT_FIELD(str_Role_Name, "roleName"),
	DEFINE_STRUCT_FIELD(c_Gender, "gender"),
	DEFINE_STRUCT_FIELD(c_Occupation, "occupation"),
	DEFINE_STRUCT_FIELD(us_Initial_Level, "initialLevel"),
	DEFINE_STRUCT_FIELD(un_User_IP, "userIp"),
	DEFINE_STRUCT_FIELD(us_User_Port, "userPort"),
	DEFINE_STRUCT_FIELD(uc_Community_ID, "communityId"),
	DEFINE_STRUCT_FIELD(c_Pad1, "pad1"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
DEFINE_STRUCT_SCHEMA(agip::SAGIPCreateRoleRes,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(n_Result_Code, "resultCode"),
	DEFINE_STRUCT_FIELD(un_Role_ID, "roleId"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
FUNCTION_BIND(CreateRole);

DEFINE_STRUCT_SCHEMA(agip::SAGIPGWDataReport,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(us_Game_ID, "gameId"),
	DEFINE_STRUCT_FIELD(us_Pad_1, "pad1"),
	DEFINE_STRUCT_FIELD(un_Gateway_ID, "gatewayId"),
	DEFINE_STRUCT_FIELD(un_Server_ID, "serverId"),
	DEFINE_STRUCT_FIELD(n_Data_Count, "dataCount"),
	DEFINE_STRUCT_FIELD(n_Data_Type, "dataType"),
	DEFINE_STRUCT_FIELD(n_Data_Value, "dataValue"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
DEFINE_STRUCT_SCHEMA(agip::SAGIPGWDataReportRes,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(n_Result_Code, "resultCode"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
FUNCTION_BIND(GWDataReport);

DEFINE_STRUCT_SCHEMA(agip::SAGIPHandset,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
DEFINE_STRUCT_SCHEMA(agip::SAGIPHandsetRes,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
FUNCTION_BIND(Handset);

DEFINE_STRUCT_SCHEMA(agip::SAGIPRoleEnterGameEx5,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(un_Server_ID, "serverId"),
	DEFINE_STRUCT_FIELD(un_User_ID, "userId"),
	DEFINE_STRUCT_FIELD(un_Role_ID, "roleId"),
	DEFINE_STRUCT_FIELD(us_Level, "level"),
	DEFINE_STRUCT_FIELD(uc_Gender, "gender"),
	DEFINE_STRUCT_FIELD(uc_Occupation_ID, "occupationId"),
	DEFINE_STRUCT_FIELD(un_Corps_ID, "corpsId"),
	DEFINE_STRUCT_FIELD(us_Community_ID, "communityId"),
	DEFINE_STRUCT_FIELD(us_Client_Port, "clientPort"),
	DEFINE_STRUCT_FIELD(un_Client_IP, "clientIp"),
	DEFINE_STRUCT_FIELD(un_Client_Type, "clientType"),
	DEFINE_STRUCT_FIELD(sz_Client_MAC, "clientMac"),
	DEFINE_STRUCT_FIELD(sz_Hardware_SN1, "hardwareSn1"),
	DEFINE_STRUCT_FIELD(sz_Hardware_SN2, "hardwareSn2"),
	DEFINE_STRUCT_FIELD(sz_Uddi, "uddi"),
	DEFINE_STRUCT_FIELD(sz_Model_Version, "modelVersion"),
	DEFINE_STRUCT_FIELD(sz_Ldid, "ldid"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
DEFINE_STRUCT_SCHEMA(agip::SAGIPRoleEnterGameEx5Res,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(n_Result_Code, "resultCode"),
	DEFINE_STRUCT_FIELD(n_Balance_Count, "balanceCount"),
	DEFINE_STRUCT_FIELD(n_Subject_ID, "subjectId"),
	DEFINE_STRUCT_FIELD(n_Amount, "amount"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
FUNCTION_BIND(RoleEnterGameEx5);

DEFINE_STRUCT_SCHEMA(agip::SAGIPUnbind,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(un_Gateway_ID, "gatewayId"),
	DEFINE_STRUCT_FIELD(checkSum, "&crc"));
DEFINE_STRUCT_SCHEMA(agip::SAGIPUnbindRes,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(n_Result_Code, "resultCode"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
FUNCTION_BIND(Unbind);

DEFINE_STRUCT_SCHEMA(agip::SAGIPUserChargeEx2,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(un_Detail_ID_H, "detailIdh"),
	DEFINE_STRUCT_FIELD(un_Detail_ID_L, "detailIdl"),
	DEFINE_STRUCT_FIELD(un_User_ID, "userId"),
	DEFINE_STRUCT_FIELD(us_Subject_ID, "subjectId"),
	DEFINE_STRUCT_FIELD(ns_Pad, "pad"),
	DEFINE_STRUCT_FIELD(n_Amount, "amount"),
	DEFINE_STRUCT_FIELD(n_Charge_Money, "chargeMoney"),
	DEFINE_STRUCT_FIELD(un_Charge_Time, "chargeTime"),
	DEFINE_STRUCT_FIELD(n_Total_Amount, "totalAmount"),
	DEFINE_STRUCT_FIELD(n_Balance, "balance"),
	DEFINE_STRUCT_FIELD(str_Attach_Code, "attachCode"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
DEFINE_STRUCT_SCHEMA(agip::SAGIPUserChargeEx2Res,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(n_Result_Code, "resultCode"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
FUNCTION_BIND(UserChargeEx2);

DEFINE_STRUCT_SCHEMA(agip::SAGIPUserItemMinusEx,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(un_User_ID, "userId"),
	DEFINE_STRUCT_FIELD(un_Role_ID, "roleId"),
	DEFINE_STRUCT_FIELD(un_Item_Count, "itemCount"),
	DEFINE_STRUCT_FIELD(un_Activity_ID, "activityId"),
	DEFINE_STRUCT_FIELD(str_Item_Code, "itemCode"),
	DEFINE_STRUCT_FIELD(n_Item_Num, "itemNum"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
DEFINE_STRUCT_SCHEMA(agip::SAGIPUserItemMinusExRes,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(n_Result_Code, "resultCode"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
FUNCTION_BIND(UserItemMinusEx);

DEFINE_STRUCT_SCHEMA(agip::SAGIPUserLogout,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(un_User_ID, "userId"),
	DEFINE_STRUCT_FIELD(un_Role_ID, "roleId"),
	DEFINE_STRUCT_FIELD(c_Logout_Flag, "logoutFlag"),
	DEFINE_STRUCT_FIELD(c_Occupation, "occupation"),
	DEFINE_STRUCT_FIELD(us_Role_Level, "roleLevel"),
	DEFINE_STRUCT_FIELD(un_Rating_ID, "ratingId"),
	DEFINE_STRUCT_FIELD(un_Money1, "money1"),
	DEFINE_STRUCT_FIELD(un_Money2, "money2"),
	DEFINE_STRUCT_FIELD(un_Experience_Higher, "experienceHigher"),
	DEFINE_STRUCT_FIELD(un_Experience_Lower, "experienceLower"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
DEFINE_STRUCT_SCHEMA(agip::SAGIPUserLogoutRes,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(n_Result_Code, "resultCode"),
	DEFINE_STRUCT_FIELD(n_Balance_Count, "balanceCount"),
	DEFINE_STRUCT_FIELD(n_Subject_ID, "subjectId"),
	DEFINE_STRUCT_FIELD(n_Amount, "amount"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
FUNCTION_BIND(UserLogout);

DEFINE_STRUCT_SCHEMA(agip::SAGIPUserBalanceInfoEx1,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(un_User_ID, "userId"),
	DEFINE_STRUCT_FIELD(un_Role_ID, "roleId"),
	DEFINE_STRUCT_FIELD(un_Rating_ID, "ratingId"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
DEFINE_STRUCT_SCHEMA(agip::SAGIPUserBalanceInfoEx1Res,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(n_Result_Code, "resultCode"),
	DEFINE_STRUCT_FIELD(un_User_ID, "userId"),
	DEFINE_STRUCT_FIELD(un_Role_ID, "roleId"),
	DEFINE_STRUCT_FIELD(n_Subject_Count, "subjectCount"),
	DEFINE_STRUCT_FIELD(n_Subject_ID, "subjectId"),
	DEFINE_STRUCT_FIELD(n_Total_Charge, "totalCharge"),
	DEFINE_STRUCT_FIELD(n_Balance, "balance"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
FUNCTION_BIND(UserBalanceInfoEx1);

DEFINE_STRUCT_SCHEMA(agip::SAGIPUserIBPayEx1,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(un_Detail_ID_H, "detailIdh"),
	DEFINE_STRUCT_FIELD(un_Detail_ID_L, "detailIdl"),
	DEFINE_STRUCT_FIELD(un_User_ID, "userId"),
	DEFINE_STRUCT_FIELD(un_Role_ID, "roleId"),
	DEFINE_STRUCT_FIELD(uc_Role_Gender, "roleGender"),
	DEFINE_STRUCT_FIELD(uc_Role_Occupation, "roleOccupation"),
	DEFINE_STRUCT_FIELD(us_Role_Level, "roleLevel"),
	DEFINE_STRUCT_FIELD(un_Rating_ID, "ratingId"),
	DEFINE_STRUCT_FIELD(sz_IB_Code, "ibCode"),
	DEFINE_STRUCT_FIELD(uc_Package_Flag, "packageFlag"),
	DEFINE_STRUCT_FIELD(c_Pad, "pad"),
	DEFINE_STRUCT_FIELD(us_IB_Count, "ibCount"),
	DEFINE_STRUCT_FIELD(n_Pay_Time, "payTime"),
	DEFINE_STRUCT_FIELD(un_User_IP, "userIp"),
	DEFINE_STRUCT_FIELD(n_Price, "price"),
	DEFINE_STRUCT_FIELD(n_Discount_Price, "discountPrice"),
	DEFINE_STRUCT_FIELD(un_Subject_Count, "subjectCount"),
	DEFINE_STRUCT_FIELD(n_Subject_ID, "subjectId"),
	DEFINE_STRUCT_FIELD(un_Sub_Amount, "subAmount"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
DEFINE_STRUCT_SCHEMA(agip::SAGIPUserIBPayEx1Res,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(n_Result_Code, "resultCode"),
	DEFINE_STRUCT_FIELD(un_Detail_ID_H, "detailIdh"),
	DEFINE_STRUCT_FIELD(un_Detail_ID_L, "detailIdl"),
	DEFINE_STRUCT_FIELD(un_User_ID, "userId"),
	DEFINE_STRUCT_FIELD(un_Rating_ID, "ratingId"),
	DEFINE_STRUCT_FIELD(un_Cost_Amount, "costAmount"),
	DEFINE_STRUCT_FIELD(un_Balance_Count, "balanceCount"),
	DEFINE_STRUCT_FIELD(n_Subject_ID, "subjectId"),
	DEFINE_STRUCT_FIELD(n_Balance, "balance"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
FUNCTION_BIND(UserIBPayEx1);

DEFINE_STRUCT_SCHEMA(agip::SAGIPDeleteUserIBPayEx,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(un_Detail_ID_H, "detailIdh"),
	DEFINE_STRUCT_FIELD(un_Detail_ID_L, "detailIdl"),
	DEFINE_STRUCT_FIELD(un_User_ID, "userId"),
	DEFINE_STRUCT_FIELD(un_Role_ID, "roleId"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
DEFINE_STRUCT_SCHEMA(agip::SAGIPDeleteUserIBPayExRes,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(n_Result_Code, "resultCode"),
	DEFINE_STRUCT_FIELD(un_Balance_Count, "balanceCount"),
	DEFINE_STRUCT_FIELD(n_Subject_ID, "subjectId"),
	DEFINE_STRUCT_FIELD(un_Balance, "balance"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));
FUNCTION_BIND(DeleteUserIBPayEx);


 
void registFunc(Napi::Env& env, Napi::Object& exports) {
	REGIST_MAP_AND_EXPORT(BIND_RES, Bind);
	REGIST_MAP_AND_EXPORT(CREATE_ROLE_RES, CreateRole);
	REGIST_MAP_AND_EXPORT(GW_DATA_REPORT_RES, GWDataReport);
	REGIST_MAP_AND_EXPORT(HANDSET_RES, Handset);
	REGIST_MAP_AND_EXPORT(ROLE_ENTER_GAME_EX5_RES, RoleEnterGameEx5);
	REGIST_MAP_AND_EXPORT(UNBIND_RES, Unbind);
	REGIST_MAP_AND_EXPORT(USER_CHARGE_EX2_RES, UserChargeEx2);
	REGIST_MAP_AND_EXPORT(USER_ITEM_MINUS_EX_RES, UserItemMinusEx);
	REGIST_MAP_AND_EXPORT(USER_LOGOUT_RES, UserLogout);
	REGIST_MAP_AND_EXPORT(USER_BALANCE_INFO_EX1_RES, UserBalanceInfoEx1);
	REGIST_MAP_AND_EXPORT(USER_IB_PAY_EX1_RES, UserIBPayEx1);
	REGIST_MAP_AND_EXPORT(DELETE_USER_IB_PAY_EX_RES, DeleteUserIBPayEx);

}