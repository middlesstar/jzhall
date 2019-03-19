//
//  AppStore_ios.cpp
//  buyu_final
//
//  Created by XiaoQuan Sha on 12/17/15.
//
//

//https://itunes.apple.com/us/app/dian-wan-cheng-bu-yu-jie-ji/id1062224929?ls=1&mt=8

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "Device.h"
#include "iOSPayHelper.h"
//#include "plazz/data/GlobalState.h"

const char *APP_URL = "http://itunes.apple.com/lookup?id=1062224929";
//const char *APP_URL = "https://itunes.apple.com/us/app/dian-wan-cheng/id1104838649?l=zh&ls=1&mt=8";

@interface JZVerManager : NSObject

@property(nonatomic, strong)NSString* trackViewURL;

+(JZVerManager*) getInstance;

-(void) openCommentUrl;

@end

static JZVerManager* mInstance = nil;
@implementation JZVerManager

+(JZVerManager*) getInstance{
    
    if(mInstance == nil){
        
        mInstance = [[JZVerManager alloc] init];
    }
    return mInstance;
}


-(void) openCommentUrl{
    
    if (self.trackViewURL != nil) {
        
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:self.trackViewURL]];
        
    }else{
        NSString *URL = [NSString stringWithFormat:@"%s", APP_URL];
        NSMutableURLRequest *request = [[NSMutableURLRequest alloc] init];
        [request setURL:[NSURL URLWithString:URL]];
        [request setHTTPMethod:@"POST"];
        NSHTTPURLResponse *urlResponse = nil;
        NSError *error = nil;
        NSData *recervedData = [NSURLConnection sendSynchronousRequest:request returningResponse:&urlResponse error:&error];
        
        if(recervedData == nil) return ;
        
        NSDictionary *dic  = [NSJSONSerialization JSONObjectWithData:recervedData options:NSJSONReadingMutableContainers error:nil];
        NSArray *infoArray  = [dic objectForKey:@"results"];
        
        if ([infoArray count]) {
            NSDictionary *releaseInfo   = [infoArray objectAtIndex:0];
            self.trackViewURL = [releaseInfo objectForKey:@"trackViewUrl"];
            [[UIApplication sharedApplication] openURL:[NSURL URLWithString:self.trackViewURL]];
        }
    }
}

@end

namespace JZ {
    
    void Device::updateGame(const char *url)
    {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];
    }
    
    void Device::pay(int rechargeType, const std::string &userid, const std::string &orderid)
    {
		iOSPayHelper::getInstance()->pay(rechargeType, userid, orderid);
    }

    bool Device::isThisVersionOnline()
    {
//        return iOSPayHelper::bOnline;
//        return GlobalState::getInstance()->updateLevel() == ReleaseVersion ||
//        GlobalState::getInstance()->updateLevel() == NoneForceUpgrade;
//        return false;
        return true;
    }

	void Device::openCommentUrl()
	{
        [[JZVerManager getInstance] openCommentUrl];
	}

    void Device::UmengInit()
    {
    	//MOBCLICKCPP_START_WITH_APPKEY(umKey_ios);
    }
}

