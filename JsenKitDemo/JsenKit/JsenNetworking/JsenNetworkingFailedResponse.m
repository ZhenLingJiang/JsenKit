//
//  JsenNetworkingFailedResponse.m
//  JsenKit
//
//  Created by Wangxuesen on 2016/11/14.
//  Copyright © 2016年 WangXuesen. All rights reserved.
//

#import "JsenNetworkingFailedResponse.h"
#import "JsenNetworkingConfig.h"

@implementation JsenNetworkingFailedResponse

- (NSString *)message {
    if (_userInfo) {
        return _userInfo[NSLocalizedDescriptionKey];
    }
    if (_message) {
        return _message;
    }
    return @"unkwon error";
}

+ (instancetype)responseWithError:(NSError *)error {
    JsenNetworkingFailedResponse *response = [[JsenNetworkingFailedResponse alloc] init];
    response.userInfo = error.userInfo;
    response.code = @(error.code);
    return response;
}

+ (instancetype)responseWithResponseObject:(NSDictionary *)responseObject {
    JsenNetworkingFailedResponse *response  = [[JsenNetworkingFailedResponse alloc] init];
    response.userInfo = nil;
    NSNumber *code = responseObject[JsenNetworkingResponseStatusCodeKeyDefine];
    if (code) {
        response.code = code;
    }
    
    NSString *message = responseObject[JsenNetworkingResponseMessageKeyDefine];
    if (message) {
        response.message = message;
    } else {
        response.message = [JsenNetworkingConfig shareConfig].customErrorStatusCode[code];
    }
    
    return response;
}
@end
