//
//  JsenNetworkingConfig.h
//  JsenKit
//
//  Created by WangXuesen on 2016/11/14.
//  Copyright © 2016年 WangXuesen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AFNetworking/AFNetworking.h>



#define JsenNetworkingResponseMessageKeyDefine [JsenNetworkingConfig shareConfig].responseFormat[JsenNetworkingResponseMessageKey]
#define JsenNetworkingResponseTimelineKeyDefine [JsenNetworkingConfig shareConfig].responseFormat[JsenNetworkingResponseTimelineKey]
#define JsenNetworkingResponseStatusCodeKeyDefine [JsenNetworkingConfig shareConfig].responseFormat[JsenNetworkingResponseStatusCodeKey]
#define JsenNetworkingResponseDataKeyDefine [JsenNetworkingConfig shareConfig].responseFormat[JsenNetworkingResponseDataKey]




//自定义的错误码发送通知时的名字,如果需要接收此通知注册此key即可，收到的object为JsenNetworkingFailedResponse 实例

extern NSString *const JsenNetworkingCustomHttpErrorNotificationKey;

/*
 responseFormat 中获取对应value的key
 */
//数据
extern NSString *const JsenNetworkingResponseDataKey;
//状态码
extern NSString *const JsenNetworkingResponseStatusCodeKey;
//消息，描述
extern NSString *const JsenNetworkingResponseMessageKey;
//时间戳
extern NSString *const JsenNetworkingResponseTimelineKey;

/**
 签名算法

 @param signKeyValues 被签名的dic
 @return signkey
 */
typedef NSString* (^JsenNetworkingConfigSignBlock)(NSDictionary *signKeyValues);

/**
 公共参数获取block

 @return 公共参数字典
 */
typedef NSDictionary* (^JsenNetworkingConfigGlobalParametersBlock) (void);

/**
 序列化类型

 - JsenNetworkingConfigSerializerHTTP: HTTP
 - JsenNetworkingConfigSerializerJSON: JSON
 */
typedef NS_ENUM(NSUInteger, JsenNetworkingConfigSerializer) {
    JsenNetworkingConfigSerializerHTTP,
    JsenNetworkingConfigSerializerJSON
};



@interface JsenNetworkingConfig : NSObject

/**
 设置host
 eg:
 完整的请求url为https://www.jsennet.com/feedback/upload和https://www.jsennet.com/feedback/download
 
 则你的host应为https://www.jsennet.com/feedback
 */
@property (nonatomic, copy) NSString *host;

/**
 请求成功自定义状态码
 在http 200的前提下，服务端一般会自定义一个表示本次请求有效的状态码
 默认为0
 */
@property (nonatomic, copy) NSNumber *customSuccessStatusCode;

/**
 每个接口对应的model配置属性
 比如:@"/feedback/upload" 接口对应的model类是 FeedBackModel.h
 
 则modelClass设置为：
 @{
    @"/feedback/upload":NSClassFromString(@"FeedBackModel")
 }
 
 */
@property (nonatomic, strong) NSDictionary *modelClass;

/**
 请求头的参数
 比如: http header的UserAgent 中添加参数
 @{
    @"User-Agent":@{
                      @"key1":@"value1",
                      @"key2":@"value2"
                    }
 }
 */
@property (nonatomic, strong) NSDictionary *httpHeader;

/**
 公共参数
 eg:
 @{
    @"screenWidth"  :@"400",
    @"screenHeight" :@"600",
    @"phone"        :@"iPhone6s",
    @"OS"           :@"iOS10.1.1",
 }
 */
@property (nonatomic, strong) NSDictionary *globalParameters;

/**
 公共参数配置block
 eg：
 return @{
     @"network":@"wifi"
 }
 */
@property (nonatomic, strong) JsenNetworkingConfigGlobalParametersBlock globalParametersBlock;

/**
 自定义的错误码及其对应的错误提示信息
 比如：
 用户登录token过期，此时调用所有的接口都会返回错误码 100018 ，
 
 设置这个属性为：
 @{
    @100018:@"token过期"，
 }
 
 ／／TODO 什么代理方法？
 
 这时就可以在代理方法中获取到自定义错误的回调
 */
@property (nonatomic, strong) NSDictionary *customErrorStatusCode;

/**
 没有网络时的自定义状态码 默认 9999
 */
@property (nonatomic, strong) NSNumber *noNetworkStatusCode;

/**
 服务端json返回响应数据格式 
 如 ：
 {
    "code": 0,
    "data": {
    "count": 1
    },
    "msg": "SUCCESS",
    "timestamp": 1465805236
 }
 
 eg:
 @{
    JsenNetworkingResponseDataKey       :@"data",
    JsenNetworkingResponseStatusCodeKey :@"code",
    JsenNetworkingResponseMessageKey    :@"message",
    JsenNetworkingResponseTimelineKey   :@"timestamp",
 }
 其中
 JsenNetworkingResponseDataKey
 JsenNetworkingResponseStatusCodeKey
 JsenNetworkingResponseMessageKey
 JsenNetworkingResponseTimelineKey
 是本类中声明的static字符串，专门用来作为key。
 */
@property (nonatomic, strong) NSDictionary *responseFormat;


/**
 默认超时时间，如果不设置则为10s
 */
@property (nonatomic, assign) NSTimeInterval defaultTimeoutInterval;

/**
 超时时间配置字典
 
 如果不设置，则为默认超时时间
 如果你的某一个接口需要特殊的超时时间，比如：
 @“/feddback/uploadImages” 接口需要100秒的超时时间
 则配置timeoutInterval为：
 
 @{
    @“/feddback/uploadImages”:@100,
 }
 
 或者
 @{
    @“/feddback/uploadImages”:@"100",
 }
 
 */
@property (nonatomic, strong) NSDictionary *timeoutInterval;

/**
 设置请求参数的序列化器类型
 默认：JsenNetworkingConfigSerializerHTTP
 */
//@property (nonatomic, assign) JsenNetworkingConfigSerializer requestSerializerType;

/**
 配置每一个借口的requestSerializerType
 eg：
 @{
     @"app/loadUserInfo":@(JsenNetworkingConfigSerializerHTTP)
 }
 
 其中 JsenNetworkingConfigSerializerHTTP 是 JsenNetworkingConfigSerializer类型
 
 */
@property (nonatomic, strong) NSDictionary *requestSerializerTypeConfig;


/**
 签名算法block
 针对有些server会设计接口签名，比如 所有的参数会做一次加密，然后把加密串作为参数一起发送出去
 server对加密串进行校验，用来判断是否是自己的客户端发送的请求。
 
 例如：
 参数：
 {
    @"name":@"jsenKit",
    @"id":@"123"
 }
 
 通过此方法加密后的串是 @"alsdjflasdjflksadfjkasdfasdf"
 那么参数就会变成
 {
    @"name":@"jsenKit",
    @"id":@"123",
    signKeyName:@"alsdjflasdjflksadfjkasdfasdf"
 }
 
 signKeyName 是 ‘@property (nonatomic, copy) NSString *signKeyName;’ 属性，请务必在实现此block后设置
 
 */
@property (nonatomic, copy) JsenNetworkingConfigSignBlock signBlock;

/**
 签名key name
 */
@property (nonatomic, copy) NSString *signKeyName;

/**
 不参与签名的接口
 */
@property (nonatomic, copy) NSDictionary *noSignAPI;

/**
 AFSecurityPolicy
 */
@property (nonatomic, strong) AFSecurityPolicy *securityPolicy;

/**
 初始化

 @return self的实例
 */
+ (instancetype)shareConfig;

/**
 apikey对应的model class

 @param apiKey apikey
 @return Class
 */
- (Class)modelClassWithAPIKey:(NSString *)apiKey;

/**
 apikey对应的超时时间，如果没有设置则是

 @param apiKey apikey
 @return 超时时间
 */
- (NSTimeInterval)timeoutIntervalWithAPIKey:(NSString *)apiKey;

/**
 根据apikey拼接完整的请求url

 @param apiKey apikey
 @return 完整的请求url
 */
- (NSString *)api:(NSString *)apiKey;

@end
