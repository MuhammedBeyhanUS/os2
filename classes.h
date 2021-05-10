#ifndef __CLASSES__
#define __CLASSES__ 
#include <vector>
#include <pthread.h>
#include "writeOutput.h"
using namespace std;
class drone{
  public:
    int id;
    int travelSpeed;
    int startingHubIdH_D;
    int rangeR_D;
};
class hub{
  public :
    int id;
    int incomingPackageStorageSizeI_H;
    int outgoingPackageStorageSizeO_H;
    int numberOfChargingSpacesForDrones;
    int isActive;
    int howManyIncomingPackagesCurrently;
    int howManyOutgoingPackagesCurrently;
    int receiverId;
    pthread_cond_t canDeposit;
    pthread_mutex_t outgoingMutex;
    vector<int> distances;
    vector<drone> currentExistingDrones;
    vector<PackageInfo*> outgoingPackages;
    vector<PackageInfo*> incomingPackages;
};
class sender{
  public: 
    int id;
    int theTimeSenderWaitsBetweenTwoPackets;
    int assignedHubIdForTheSender;
    int totalNumberOfPackagesSenderWillSend;
    int totalHubs;
};
/**
 * @brief Continues until the hub they are assigned to, goes on
 * 
 */
class receiver{
  public:
    int id;
    int timeReceiverWaitsBetweenTwoPacketsS_R;
    int assignedHubIdH_R;
    
};

#endif