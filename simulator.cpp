#include <iostream> // std::cout
#include <pthread.h>   // std::thread
// #include <stdio.h>
#include <cstdlib>
#include "writeOutput.h"
#include "helper.h"
#include <semaphore.h>
#include <vector>
#include "classes.h"
// #include <semaphore.h>


using namespace std;
hub* hubs;


receiver * receivers;
int number_of_hub_N_t;
//sem_t semaphore;

int isDebugged=1;
//print
void p(const char stri[]){
  if(isDebugged){
    cout << stri<<"\n";
  }
}






int thereAreActiveHubs(){
  for(int i=0;i<number_of_hub_N_t;i++){
    if(hubs[i].isActive)
      return 1;
  }
  return 0;
}
int checkIfThereAreAnyIncomingPackagesOnTheHub(int hubId){
  return hubs[hubId].howManyIncomingPackagesCurrently;
}


void ReceiverPickUp(int hubId){
//  hubs[hubId].howManyIncomingPackagesCurrently--;
}

void * receiverThread(void * receiverDetailsAsVoid){
    receiver *r = (receiver*) receiverDetailsAsVoid;
    int ID=r->id, Speed=r->timeReceiverWaitsBetweenTwoPacketsS_R,AssignedHub=r->assignedHubIdH_R;
    int CurrentHub = AssignedHub;
    ReceiverInfo* receiverInfo  = new ReceiverInfo;
    FillReceiverInfo(receiverInfo, ID, CurrentHub, NULL);
    WriteOutput(NULL, receiverInfo, NULL, NULL, RECEIVER_CREATED);
    while(hubs[CurrentHub].isActive){
        p("There are active hubs");
        //Do some stuff
        int packages = checkIfThereAreAnyIncomingPackagesOnTheHub(r->assignedHubIdH_R);
        for(int i=0;i<packages;i++){
          PackageInfo* package = hubs[CurrentHub].incomingPackages.back();
          hubs[CurrentHub].incomingPackages.pop_back();
          ReceiverPickUp(r->assignedHubIdH_R);

          FillPacketInfo(package,package->sender_id,package->sending_hub_id,package->receiver_id,package->receiving_hub_id);
          
          FillReceiverInfo(receiverInfo,r->id,r->assignedHubIdH_R,package);
          WriteOutput(NULL, receiverInfo, NULL, NULL,RECEIVER_PICKUP);
          wait(r->timeReceiverWaitsBetweenTwoPacketsS_R);
        }
        wait(1000);
    }

/**
CurrentHub ← AssignedHub
FillReceiverInfo(ReceiverInfo, ID, CurrentHub, NULL)
WriteOutput(NULL, ReceiverInfo, NULL, NULL, RECEIVER_CREATED)
while there are active hubs do
    Check if there any (incoming) packages for the receiver on the hub
    for each of the packages do
        ReceiverPickUp (Package)
        FillPacketInfo(PackageInfo, SenderID, SendingHub, ID, CurrentHub)
        FillReceiverInfo(ReceiverInfo, ID, CurrentHub, PackageInfo)
        WriteOutput(NULL, ReceiverInfo, NULL, NULL,RECEIVER_PICKUP)
        Sleep for duration given for the receiver (see Input Specifications)
    end
end
FillReceiverInfo(ReceiverInfo, ID, CurrentHub, NULL)
WriteOutput(NULL, ReceiverInfo, NULL, NULL, RECEIVER_STOPPED)



The functions are explained below:
• ReceiverPickUp: Informs the hub that the package has been picked up from the in-
coming package storage. This means that if there are drones waiting to deliver to this
hub, one of them can deliver at that point.
 */
    FillReceiverInfo(receiverInfo, ID,CurrentHub, NULL);
    WriteOutput(NULL, receiverInfo, NULL, NULL,RECEIVER_STOPPED);
    delete receiverInfo;
    return NULL;
}


int selectARandomHubAndItsReceiverToSendAPackageTo(int totalNumberOfHubs){

	return rand()%totalNumberOfHubs;
}
void WaitCanDeposit(int hubId){
  //pthread_mutex_lock();// hub'a birden fazla sender ayni anda dalmamali
  if(hubs[hubId].outgoingPackageStorageSizeO_H>hubs[hubId].howManyIncomingPackagesCurrently){
    //do nothing and deposit it
  }else{
  //sem_wait//beklemesi gerek bos yer olmasini
  pthread_cond_wait(&hubs[hubId].canDeposit,&hubs[hubId].outgoingMutex);
  }
  //pthread_mutex_unlock
}


int getReceiverIdFromHub(int hubNu){
  for(int i=0;i<number_of_hub_N_t;i++){
    if(receivers[i].assignedHubIdH_R==hubNu+1){
      return receivers[i].id;
    }
  }
  return 0;

}
void SenderStopped(int senderId){
  //TODO: implement;
}

void* senderThread(void* senderDetailsAsVoid){
	sender *s = (sender*) senderDetailsAsVoid;
	int ID = s->id,Speed = s->theTimeSenderWaitsBetweenTwoPackets,TotalPackages=s->totalNumberOfPackagesSenderWillSend,AssignedHub=s->assignedHubIdForTheSender;
	//TODO total number of hubs
	int CurrentHub = AssignedHub;
	int RemainingPackages= TotalPackages;
	SenderInfo* senderInfo = new SenderInfo;
	FillSenderInfo(senderInfo, ID,CurrentHub,RemainingPackages, NULL);
	WriteOutput(senderInfo, NULL, NULL, NULL,SENDER_CREATED);
	while(RemainingPackages>0){ //there are remaining packages
		int randomHubId = selectARandomHubAndItsReceiverToSendAPackageTo(s->totalHubs);
    // todo wait

    PackageInfo *packet = new PackageInfo;
		FillPacketInfo(packet, ID, CurrentHub, hubs[randomHubId].receiverId, randomHubId);
    SenderInfo * senderInfo;
    FillSenderInfo(senderInfo, ID, CurrentHub, RemainingPackages, packet);
    hubs[randomHubId].outgoingPackages.push_back(packet);
    WriteOutput(senderInfo, NULL, NULL, NULL,SENDER_STOPPED);
    s->totalNumberOfPackagesSenderWillSend--;
    wait(s->theTimeSenderWaitsBetweenTwoPackets);
		break;
		//sem_wait()
	}

	delete senderInfo;
	return NULL;
}





void WaitUntilPackageDeposited(int hubID){
  //TODO implement
}
drone SelectTheDroneWithTheHighestRange(int hubID){
  //TODO implement
  return hubs[hubID].currentExistingDrones.back();
}
void AssignAndNotifyDrone(PackageInfo package, drone d){
  //TODO implement
}
bool CallDroneFromHubs(int hubId){
  //TODO implement
  //drone varsa true don, yoksa false don
  return false;
}
void WaitTimeoutOrDrone(){
  wait(1);
}
void HubStopped(int hubID){
  //TODO implement
}
/*
Data: ID, IncomingSize, OutgoingSize, ChargingSize
FillHubInfo(HubInfo, ID)
WriteOutput(NULL, NULL, NULL, HubInfo, HUB_CREATED)
while there are active senders or packages in either storage do
    WaitUntilPackageDeposited ()
    start:
    if There are drones in the hub then
        Select the drone with the highest current range
        AssignAndNotifyDrone (Package, Drone)
    else
    CallDroneFromHubs ()
        if No drone is found in other hubs then
            WaitTimeoutOrDrone ()
            goto start
        end
    end
end
HubStopped ()
FillHubInfo(HubInfo, ID)
WriteOutput(NULL, NULL, NULL, HubInfo, HUB_STOPPED)


The functions are explained below:
• WaitUntilPackageDeposited: Waits until a package is deposited from a sender.
• AssignAndNotifyDrone: Assign the package to the drone to be delivered to the desti-
nation. After assigning the package, notify the drone for delivery.
• CallDroneFromHubs: Starting from the closest hub to the farthest, call a drone until
one is found. Only one drone should be called at the end if there is any available in the
other hubs.
• WaitTimeoutOrDrone: Wait a specific duration until a drone arrives. The specific
duration is 1 units of time. Time units will be explained later.
• HubStopped: Notify all other simulation objects that this hub is not active anymore.
 */

void* hubThread(void* hubDetails){
    auto hubDetailsAsHub = (hub*)hubDetails;
    int ID=hubDetailsAsHub->id, IncomingSize=hubDetailsAsHub->incomingPackageStorageSizeI_H, OutgoingSize = hubDetailsAsHub->outgoingPackageStorageSizeO_H, ChargingSize = hubDetailsAsHub->numberOfChargingSpacesForDrones;
    
    //pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;
    //pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    HubInfo* hubInfo = new HubInfo;
    FillHubInfo(hubInfo, ID);
    WriteOutput(NULL, NULL, NULL, hubInfo, HUB_CREATED);
    bool gotoStart=false;
    while(thereAreActiveHubs()){ //TODO //there are active senders or packages in either storage do
        //pthread_cond_wait(&condition_var,&mutex);
        if(!gotoStart)
          WaitUntilPackageDeposited(ID);

        //start:
        
        if(hubDetailsAsHub->currentExistingDrones.size()>0){
          drone d = SelectTheDroneWithTheHighestRange(ID);
          PackageInfo p ;
          AssignAndNotifyDrone(p,d);
          gotoStart=false;
          
        }else{
          bool droneExists = CallDroneFromHubs(ID);
          WaitTimeoutOrDrone();
          gotoStart=true;
          //gotoStart()
        }
        break;

    }
    HubStopped(ID);
    FillHubInfo(hubInfo, ID);
    WriteOutput(NULL, NULL, NULL, hubInfo,HUB_STOPPED);

    delete hubInfo;
    /**
     * @brief 
Data: ID, IncomingSize, OutgoingSize, ChargingSize
FillHubInfo(HubInfo, ID)
WriteOutput(NULL, NULL, NULL, HubInfo, HUB_CREATED)
while there are active senders or packages in either storage do
     * 
     */
    return NULL;
}
/*
Data: ID, Speed, StartingHub, MaximumRange
CurrentHub ← StartingHub
CurrentRange ← M aximumRange 
FillDroneInfo(DroneInfo, ID, CurrentHub, CurrentRange, NULL, 0 )
WriteOutput(NULL, NULL, DroneInfo, NULL, DRONE_CREATED)
while there are active hubs do
    WaitSignalFromHub ()
    if The current hub assigns a package to be delivered to another then
        WaitAndReserveDestinationSpace (DestinationHub)
        WaitForRange ()
        CurrentRange ← min(CurrentRange + ChargedRange, M aximumRange)
        TakePackageFromHub (Package)
        FillPacketInfo(PackageInfo, SenderID, SendingHub, ReceiverID,
        ReceivingHub)
        FillDroneInfo(DroneInfo, ID, CurrentHub, CurrentRange, PackageInfo, 0 )
        WriteOutput(NULL, NULL, DroneInfo, NULL, DRONE_PICKUP)
        Sleep the duration of travel (duration calculation will be given later)
        CurrentRange ← CurrentRange − (Distance/Speed)
        CurrentHub ← DestinationHub
        DropPackageToHub (Package)
        FillPacketInfo(PackageInfo, SenderID, SendingHub, ReceiverID,
        ReceivingHub)
        FillDroneInfo(DroneInfo, ID, CurrentHub, CurrentRange, PackageInfo, 0 )
        WriteOutput(NULL, NULL, DroneInfo, NULL, DRONE_DEPOSITED)
    end
else if A nearby hub is requesting a drone for package delivery then
    WaitAndReserveChargingSpace (DestinationHub)
    WaitForRange ()
    CurrentRange ← min(CurrentRange + ChargedRange, M aximumRange)
    FillDroneInfo(DroneInfo, ID, CurrentHub, CurrentRange, NULL,
    DestinationHub)
    WriteOutput(NULL, NULL, DroneInfo, NULL, DRONE_GOING)
    Sleep the duration of travel (duration calculation will be given later)
    CurrentRange ← CurrentRange − (Distance/Speed)
    CurrentHub ← DestinationHub
    FillDroneInfo(DroneInfo, ID, CurrentHub, CurrentRange, NULL, 0 )
    WriteOutput(NULL, NULL, DroneInfo, NULL, DRONE_ARRIVED)
    end
end
FillDroneInfo(DroneInfo, ID, CurrentHub, CurrentRange, NULL, 0 )
WriteOutput(NULL, NULL, DroneInfo, NULL, DRONE_STOPPED)

The functions are explained below:
• WaitSignalFromHub: Waits signal from the hub indicating whether a package has been
deposited or a nearby hub is requesting a drone.
• WaitDestinationSpace: Waits until an incoming package and charging space is avail-
able at the destination hub and reserves them.
• WaitChargingSpace: Waits until a charging space is available at the destination hub
and reserves it.
• WaitForRange: Waits until the current range of the drone is sufficient for the travel
between hubs.
• TakePackageFromHub: Signal to the hub that an outgoing package has been picked up.
• DropPackageToHub: Signal to the destination hub that an incoming package has arrived.
 */

void* droneThread(void * droneThreadDeatils){
    auto droneClass = (drone*) droneThreadDeatils;
    int ID = droneClass->id;
    int Speed = droneClass->travelSpeed;
    int StartingHub=droneClass->startingHubIdH_D;
    int MaximumRange=droneClass->rangeR_D;

    int CurrentHub = StartingHub;
    int CurrentRange = MaximumRange;
    DroneInfo* droneInfo = new DroneInfo;
    FillDroneInfo(droneInfo, ID, CurrentHub, CurrentRange, NULL, 0 );
    WriteOutput(NULL,NULL, droneInfo,NULL,DRONE_CREATED);
    while(thereAreActiveHubs()){
      wait(1);
    }
    /**
     * @brief 
     * 
     * WriteOutput(NULL, NULL, DroneInfo, NULL, DRONE_CREATED)
     * while there are active hubs do
     * WaitSignalFromHub ()
         * if The current hub assigns a package to be delivered to another then

     * 
     */
    FillDroneInfo(droneInfo, ID,CurrentHub,CurrentRange, NULL, 0);
    WriteOutput(NULL, NULL, droneInfo, NULL,DRONE_STOPPED);
    delete droneInfo;
    return NULL;
}


int main()
{
  p("basladi\n");
  cin >> number_of_hub_N_t;
  //Getting hubs
  hubs = new hub[number_of_hub_N_t];
  for (int i = 0; i < number_of_hub_N_t; i++)
  {
    p("as61\n");
    hubs[i].id = i + 1;
    cin >>hubs[i].incomingPackageStorageSizeI_H;
    cin >> hubs[i].outgoingPackageStorageSizeO_H;
    cin >> hubs[i].numberOfChargingSpacesForDrones;
    hubs[i].isActive=1;
    hubs[i].howManyIncomingPackagesCurrently = 0;
    hubs[i].howManyOutgoingPackagesCurrently = 0;
    pthread_cond_init(&hubs[i].canDeposit,NULL);
    pthread_mutex_init(&hubs[i].outgoingMutex,NULL);
    for (int j = 0; j < number_of_hub_N_t; j++)
    {
      int tempDistance;
      cin >> tempDistance;
      hubs[i].distances.push_back(tempDistance);
    }
  }
  //Getting senders
  sender senders[number_of_hub_N_t];
  for (int i = 0; i < number_of_hub_N_t; i++)
  {
    p("as81\n");
    senders[i].id = i + 1;
    cin >> senders[i].theTimeSenderWaitsBetweenTwoPackets;
    cin >> senders[i].assignedHubIdForTheSender;
    cin >> senders[i].totalNumberOfPackagesSenderWillSend ;
    senders[i].totalHubs = number_of_hub_N_t;
  }
  //Getting receivers
  receivers = new receiver[number_of_hub_N_t];
  for (int i = 0; i < number_of_hub_N_t; i++)
  {
    receivers[i].id=i + 1;
    int temp;
    cin >> temp;
    receivers[i].timeReceiverWaitsBetweenTwoPacketsS_R = temp;
    cin >> receivers[i].assignedHubIdH_R;
  }
  int numberOfDrones;
  cin >> numberOfDrones;
  drone drones[numberOfDrones];
  for (int i = 0; i < numberOfDrones; i++)
  {
    drones[i].id = i + 1;
    cin >> drones[i].travelSpeed;
    cin >> drones[i].startingHubIdH_D;
    cin >> drones[i].rangeR_D;
  }
  p("as106\n");
  
  ///////////////////////////////////Create Threads
  InitWriteOutput();
  pthread_t droneThreads[numberOfDrones];
  for (int i = 0; i < numberOfDrones; i++)
  {
    drone* dronePointer=&drones[i];
    pthread_create(&droneThreads[i], NULL,droneThread,(void*)dronePointer);
    //droneThreads[i] = std::thread(droneThread, drones[i].id, drones[i].travelSpeed, drones[i].startingHubIdH_D, drones[i].rangeR_D);
    // thread thread_drone(droneThread,drones[i].id,drones[i].travelSpeed, drones[i].startingHubIdH_D, drones[i].rangeR_D);
  }
  pthread_t hubThreads[number_of_hub_N_t];
  for(int i=0;i<number_of_hub_N_t;i++){
    hub *h = &hubs[i];
    pthread_create(&hubThreads[i],NULL,hubThread,(void*)h);
    //hubThreads[i] = thread(hubThread, h.id, h.incomingPackageStorageSizeI_H, h.outgoingPackageStorageSizeO_H, h.numberOfChargingSpacesForDrones,h);
  }
  pthread_t receiverThreads[number_of_hub_N_t];
  for(int i=0;i<number_of_hub_N_t;i++){
    receiver *r = &receivers[i];
    pthread_create(&receiverThreads[i],NULL,receiverThread,(void*)r);
    //receiverThreads[i] = thread(receiverThread,r.id,r.timeReceiverWaitsBetweenTwoPacketsS_R,r.assignedHubIdH_R);
  }
  pthread_t senderThreads[number_of_hub_N_t];
  for(int i=0;i<number_of_hub_N_t;i++){
    sender *s = &senders[i];
    pthread_create(&senderThreads[i],NULL,senderThread,(void*)s);
    //senderThreads[i] = thread(senderThread,s.id,s.theTimeSenderWaitsBetweenTwoPackets,s.totalNumberOfPackagesSenderWillSend,s.assignedHubIdForTheSender    ,number_of_hub_N_t);
  }
  
  
  
  
  
  
  
  
  ////////////////////*******************JOIN Threads*************************///////////////////////// 
  
  
  
  
  
  
  
  
  
  for (int i = 0; i < numberOfDrones; i++)
  {
    pthread_join(droneThreads[i],NULL);
    //droneThreads[i].join();
  }
  for(int i=0;i<number_of_hub_N_t;i++){
    pthread_join(hubThreads[i],NULL);
    pthread_join(receiverThreads[i],NULL);
    pthread_join(senderThreads[i],NULL);
  }


  return 0;
}