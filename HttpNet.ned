//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//
// Model of the network, consisting of several clients, a server and a cloud

network HTTPNet
{
    parameters:
        int numClients @prompt("Number of clients:") = default(3);
        @display("bgb=992,619");

   	//types:
        //channel C extends ned.DatarateChannel {
               //datarate = 100Mbps;//
    submodules:
        server: HTTPServer {
            parameters:
                @display("p=148,42");
                //addr = 0;
            gates:
                voiceFlow[1];
        }
        cloud: Cloud {
            parameters:
                @display("p=148,158");
            gates:
                voiceServer[1];
                voicePresenter[1];
                voiceListeners[numClients+0];
        }
        Presenter: HTTPClient {
            parameters:
                @display("p=375,29");
                addr = 1;
                srvAddr = 0;
                clientRole = 1;
                clientsCount = numClients;
            gates:
                voiceUpload[1];
                voiceP2P[3]; // to be modified

        }
        Students[numClients]: HTTPClient {
            parameters:
                @display("p=382,283,col");
                //add new singal and statistic parameter for survey purpose
                @signal[messagelength](type=int);
                @statistic[messageLegth](record= max,timeavg,vector?);
                @statistic[dropcount](source=count(drop); record=last,vector?);
                @statistic[droprate](source=count(drop)/count(pk); record=last,vector?);


                //@display("p=382,283,col");
                @display("p=,,m,2,100,100");
                addr = 2+index;
                srvAddr = 0;
                clientRole = 0;
                clientsCount = numClients;
            gates:
                voiceUpload[1];
                voiceP2P[2*numClients+index];
                p2pin[];
                p2pout[];

        }
        Relayers: Cloud {
            parameters:
                @display("p=642,155;i=device/lan;is=l");
            gates:
                voiceServer[1];
                voicePresenter[1];
                voiceListeners[numClients+0];
                voiceRelayer[2];
        }
     //   Relayers2: Cloud {
     //       parameters:
     //           @display("p=497,155;i=device/lan;is=l");
    //        gates:
    //            voiceServer[1];
   //             voicePresenter[1];
    //            voiceListeners[numClients+0];
    //            voiceRelayer[2];
    //    }
    connections allowunconnected:
        server.voiceFlow[0] <--> cloud.voiceServer[0];
        Presenter.voiceUpload[0] <--> cloud.voicePresenter[0];

        Students[i].voiceUpload[0] <--> cloud.voiceListeners[i] for i=0..numClients-1;
        Presenter.voiceP2P[0] <--> Relayers.voicePresenter[0];
        //Presenter.voiceP2P[1] <--> Relayers2.voicePresenter[0];
        //Relayers.voiceRelayer[0]  <--> Relayers2.voiceRelayer[0];
        
        Relayers.voiceListeners[i] <--> Students[i].voiceP2P[numClients] for i=0..numClients-1;
      //  Relayers2.voiceListeners[i] <--> Students[i].voiceP2P[numClients+1] for i=0..numClients-1;

        //p2p part configure
       // Students[i].voiceP2P++ <--> Students[i+1].voiceP2P++ for i=0..numClients-2;

     //  Students[0].voiceP2P[0] <--> Students[1].voiceP2P[0] if numClients==2;

	//	for i=0..numClients-1, for j = (i*numClients)..(i+1)*numClients-1 {
//              //node[i].out[j] --> node[rightNodeIndex(i,j)].in[j];
//            //  Students[i].p2pout[j]-->Students[rightNodeIndex(i,j)].p2pin[j];
//              
          //   Students[i].voiceP2P[j] <--> Students[i].voiceP2P[j] if ;
             //Students[i].voiceP2P[j] <--> Students[i#(1<<j)].voiceP2P[j] if (i < i#(1<<j));

       //    }

        for i=0..(numClients-2), for j=(i+1)..(numClients-1) {
            Students[i].voiceP2P[j] <--> { @display("ls=red, 3"); } <--> Students[j].voiceP2P[i];
        }
}


