//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


//
// A real-time interface
//
simple HTTPClient
{
    parameters:
        int addr;
        int srvAddr;
        int clientRole;
        int clientsCount;
        volatile double sendIaTime @unit(s);
        double propDelay @unit(s);
        @display("i=device/pc2");
        @signal[endToEndDelay](type="simtime_t");
        @signal[hopCount](type="long");
        @signal[sourceAddress](type="long");
        @statistic[endToEndDelay](title="end-to-end delay of arrived packets";unit=s;record=vector,mean,max;interpolationmode=none);
        @statistic[hopCount](title="hop count of arrived packets";interpolationmode=none;record=vector?,mean,max);
        @statistic[sourceAddress](title="source address of arrived packets";interpolationmode=none;record=vector?);
    gates:
       // input voiceFromCloud[];
       // input voiceFromPresenter[];
       // output voicePush[];
        //output voiceUpload[];
        
        inout voiceUpload[];
        inout voiceP2P[];
        input p2pin[];
        output p2pout[];
        //inout voiceFromPresenter[];
        //inout voiceFromCloud[];
        
}



