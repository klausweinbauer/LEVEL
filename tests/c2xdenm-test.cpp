#include <c2xdenm.h>
#include <gtest/gtest.h>

TEST(DENM_Basic, Create_And_Delete_Message)
{
    int retC = c2x::createDENM(1, 1);
    int retD = c2x::deleteDENM(1, 1);
    ASSERT_EQ(0, retC);
    ASSERT_EQ(0, retD);
}

TEST(DENM_Basic, Create_DENM_With_The_Same_ID_Twice)
{
    int ret1 = c2x::createDENM(1, 1);
    int ret2 = c2x::createDENM(1, 1);
    c2x::deleteDENM(1, 1);
    ASSERT_EQ(ret1, 0);
    ASSERT_EQ(ret2, ERR_DENM_ALREADY_EXISTS);
}

TEST(DENM_Basic, Double_Delete_DENM)
{
    int id = c2x::createDENM(1, 1);
    int ret1 = c2x::deleteDENM(1, 1);
    int ret2 = c2x::deleteDENM(1, 1);
    ASSERT_EQ(0, ret1);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, ret2);
}

TEST(DENM_Coding, Encode_DENM_Message)
{
    c2x::createDENM(1, 1);
    uint8_t buffer[4096];

    int ret = c2x::encodeDENM(1, 1, buffer, 4096, nullptr);

    ASSERT_GT(ret, 0);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_Coding, Decode_DENM_And_Override_Message)
{
    int id = c2x::createDENM(1, 1);
    c2x::setDENMHeader(1, 1, 1, 2);

    uint8_t buffer[4096];
    int size = c2x::encodeDENM(1, 1, buffer, 4096, nullptr);
    int newProtVers, newMsgId, newStatId, newSeqNr;
    c2x::decodeDENM(&newStatId, &newSeqNr, buffer, size);
    c2x::getDENMHeader(newStatId, newSeqNr, &newProtVers, &newMsgId);

    ASSERT_EQ(1, newProtVers);
    ASSERT_EQ(2, newMsgId);
    ASSERT_EQ(1, newStatId);
    ASSERT_EQ(1, newSeqNr);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_Network, Start_And_Stop_Receiver)
{
    int retStart1 = c2x::startDENMReceiver(1998);
    int retStop1 = c2x::stopDENMReceiver();
    int retStart2 = c2x::startDENMReceiver(1998);
    int retStop2 = c2x::stopDENMReceiver();

    ASSERT_EQ(0, retStart1);
    ASSERT_EQ(0, retStop1);
    ASSERT_EQ(0, retStart2);
    ASSERT_EQ(0, retStop2);
}

TEST(DENM_Coding, Decode_New_Message)
{
    c2x::createDENM(1, 1);
    uint8_t buffer[4000];
    int size;
    c2x::encodeDENM(1, 1, buffer, 4000, &size);
    c2x::deleteDENM(1, 1);

    int id, seqNr;
    int ret = c2x::decodeDENM(&id, &seqNr, buffer, size);
    int retDel = c2x::deleteDENM(1, 1);
    ASSERT_EQ(1, id);
    ASSERT_EQ(1, seqNr);
    ASSERT_EQ(0, ret);
    ASSERT_EQ(0, retDel);
}

TEST(DENM_SituationContainer, Basic_Settings)
{
    c2x::createDENM(1, 1);
    c2x::setDENMSituationContainer(1, 1, 7, 9, 11);
    int infQual, cc, scc;
    c2x::getDENMSituationContainer(1, 1, &infQual, &cc, &scc);
    ASSERT_EQ(7, infQual);
    ASSERT_EQ(9, cc);
    ASSERT_EQ(11, scc);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_SituationContainer, Linked_Cause)
{
    c2x::createDENM(1, 1);
    c2x::setDENMSituationContainerLinkedCause(1, 1, 13, 17);
    int cc, scc;
    c2x::getDENMSituationContainerLinkedCause(1, 1, &cc, &scc);
    ASSERT_EQ(13, cc);
    ASSERT_EQ(17, scc);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_SituationContainer, Event_History)
{
    c2x::createDENM(1, 1);
    c2x::addDENMSituationContainerEventPoint(1, 1, 1, 2, 3, 4, 5);
    c2x::addDENMSituationContainerEventPoint(1, 1, 6, 7, 8, 9, 10);
    int history[100];
    int actualSize;
    c2x::getDENMSituationContainerEventHistory(1, 1, history, 100, &actualSize);
    ASSERT_EQ(10, actualSize);
    ASSERT_EQ(6, history[0]);
    ASSERT_EQ(7, history[1]);
    ASSERT_EQ(8, history[2]);
    ASSERT_EQ(9, history[3]);
    ASSERT_EQ(10, history[4]);
    ASSERT_EQ(1, history[5]);
    ASSERT_EQ(2, history[6]);
    ASSERT_EQ(3, history[7]);
    ASSERT_EQ(4, history[8]);
    ASSERT_EQ(5, history[9]);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_SituationContainer, Get_Empty_Event_History)
{
    c2x::createDENM(1, 1);
    int history[100];
    int actualSize;
    c2x::getDENMSituationContainerEventHistory(1, 1, history, 100, &actualSize);
    ASSERT_EQ(0, actualSize);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_LocationContainer, Set_And_Get_Traces)
{
    c2x::createDENM(1, 1);
    int trace1[4] = {1, 2, 3, 4};
    int trace2[8] = {11, 12, 13, 14, 15, 16, 17, 18};
    int trace1_out[4];
    int trace2_out[8];
    c2x::addDENMLocationContainerTrace(1, 1, trace1, 4);
    c2x::addDENMLocationContainerTrace(1, 1, trace2, 8);
    c2x::getDENMLocationContainerTrace(1, 1, 0, trace1_out, 4, nullptr);
    c2x::getDENMLocationContainerTrace(1, 1, 1, trace2_out, 8, nullptr);
    ASSERT_EQ(1, trace1_out[0]);
    ASSERT_EQ(2, trace1_out[1]);
    ASSERT_EQ(3, trace1_out[2]);
    ASSERT_EQ(4, trace1_out[3]);
    ASSERT_EQ(11, trace2_out[0]);
    ASSERT_EQ(12, trace2_out[1]);
    ASSERT_EQ(13, trace2_out[2]);
    ASSERT_EQ(14, trace2_out[3]);
    ASSERT_EQ(15, trace2_out[4]);
    ASSERT_EQ(16, trace2_out[5]);
    ASSERT_EQ(17, trace2_out[6]);
    ASSERT_EQ(18, trace2_out[7]);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_LocationContainer, Get_Trace_With_Wrong_Size)
{
    c2x::createDENM(2, 1);
    int trace[8] = {11, 12, 13, 14, 15, 16, 17, 18};
    int trace_out[4];
    c2x::addDENMLocationContainerTrace(2, 1, trace, 8);
    int getRet = c2x::getDENMLocationContainerTrace(2, 1, 0, trace_out, 4, nullptr);
    ASSERT_EQ(0, getRet);
    ASSERT_EQ(11, trace_out[0]);
    ASSERT_EQ(12, trace_out[1]);
    ASSERT_EQ(13, trace_out[2]);
    ASSERT_EQ(14, trace_out[3]);
    c2x::deleteDENM(2, 1);
}

TEST(DENM_LocationContainer, Get_Trace_With_Index_Out_Of_Range)
{
    c2x::createDENM(3, 1);
    int trace_out[4] = {0,0,0,0};
    c2x::addDENMLocationContainerTrace(3, 1, trace_out, 4);
    int getRet = c2x::getDENMLocationContainerTrace(3, 1, 1, trace_out, 4, nullptr);
    ASSERT_EQ(ERR_INDEX_OUT_OF_RANGE, getRet);
    c2x::deleteDENM(3, 1);
}

TEST(DENM_LocationContainer, Clear_Traces)
{
    c2x::createDENM(4, 1);
    int trace1[4] = {1,2,3,4};
    int trace2[4] = {5,6,7,8};
    int trace_out[4];
    c2x::addDENMLocationContainerTrace(4, 1, trace1, 4);
    c2x::clearDENMLocationContainerTraces(4, 1);
    c2x::addDENMLocationContainerTrace(4, 1, trace2, 4);
    c2x::getDENMLocationContainerTrace(4, 1, 0, trace_out, 4, nullptr);
    ASSERT_EQ(5, trace_out[0]);
    ASSERT_EQ(6, trace_out[1]);
    ASSERT_EQ(7, trace_out[2]);
    ASSERT_EQ(8, trace_out[3]);
    c2x::deleteDENM(4, 1);
}

TEST(DENM_LocationContainer, Set_And_Get_Speed)
{
    c2x::createDENM(1, 1);
    int val, conf;
    c2x::setDENMLocationContainerSpeed(1, 1, 7, 9);
    c2x::getDENMLocationContainerSpeed(1, 1, &val, &conf);
    ASSERT_EQ(7, val);
    ASSERT_EQ(9, conf);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_LocationContainer, Set_And_Get_Heading)
{
    c2x::createDENM(1, 1);
    int val, conf;
    c2x::setDENMLocationContainerHeading(1, 1, 7, 9);
    c2x::getDENMLocationContainerHeading(1, 1, &val, &conf);
    ASSERT_EQ(7, val);
    ASSERT_EQ(9, conf);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_LocationContainer, Set_And_Get_Road_Type)
{
    c2x::createDENM(1, 1);
    int type;
    c2x::setDENMLocationContainerRoadType(1, 1, 7);
    c2x::getDENMLocationContainerRoadType(1, 1, &type);
    ASSERT_EQ(7, type);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_LocationContainer, Test_Actual_Trace_Length)
{
    c2x::createDENM(1, 1);
    int trace[8] = {1,2,3,4,5,6,7,8};
    int trace_out[4];
    int actualLen;
    c2x::addDENMLocationContainerTrace(1, 1, trace, 8);
    c2x::getDENMLocationContainerTrace(1, 1, 0, trace_out, 4, &actualLen);
    ASSERT_EQ(8, actualLen);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_LocationContainer, Clear_Uninitialized_Trace_History) 
{
    c2x::createDENM(1, 1);
    int ret = c2x::clearDENMLocationContainerTraces(1, 1);
    ASSERT_EQ(0, ret);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_LocationContainer, Clear_Empty_Trace_History) 
{
    c2x::createDENM(1, 1);
    c2x::setDENMLocationContainerSpeed(1, 1, 0, 0);
    int ret = c2x::clearDENMLocationContainerTraces(1, 1);
    ASSERT_EQ(0, ret);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_AlacarteContainer_Errors)
{
    int msgNotFoundSet = c2x::setDENMAlacarteContainer(0, 0, 0, 0, 0);
    int msgNotFoundGet = c2x::getDENMAlacarteContainer(0, 0, nullptr, nullptr, nullptr);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMAlacarteContainer(1, 1, nullptr, nullptr, nullptr);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_AlacarteContainer)
{
    c2x::createDENM(1, 1);
    int a, b, c; 
    int retSet = c2x::setDENMAlacarteContainer(1, 1, 2, 3, 4);
    int retGet = c2x::getDENMAlacarteContainer(1, 1, &a, &b, &c);
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(2, a);
    ASSERT_EQ(3, b);
    ASSERT_EQ(4, c);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_ImpactReductionContainer_Errors)
{
    int msgNotFoundSet = c2x::setDENMImpactReductionContainer(0, 0, 0, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, nullptr, 0, 0, 0);
    int msgNotFoundGet = c2x::getDENMImpactReductionContainer(0, 0, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 
        nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, nullptr);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMImpactReductionContainer(1, 1, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 
        nullptr, nullptr, nullptr, nullptr, 0, nullptr, nullptr);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int ircNull = c2x::getDENMImpactReductionContainer(1, 1, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 
        nullptr, nullptr, nullptr, nullptr, 0, nullptr, nullptr);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, ircNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_ImpactReductionContainer)
{
    c2x::createDENM(1, 1);
    int pillarPos[2] = { 99, 98 };
    uint8_t posOcc[2] = { 97, 96 };
    int a, b, c, d, e, f, g, h, i, j, k, l, m, n, o;
    int pillarPos_out[2];
    uint8_t posOcc_out[2];
    int retSet = c2x::setDENMImpactReductionContainer(1, 1, 1, 2, 3, 4, pillarPos, 2, 5, 6, 7, 8, posOcc, 2, 9, 10);
    int retGet = c2x::getDENMImpactReductionContainer(1, 1, &a, &b, &c, &d, pillarPos_out, 2, &e, &f, &g, &h, posOcc_out, 2, &i, &j);
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(1, a);
    ASSERT_EQ(2, b);
    ASSERT_EQ(3, c);
    ASSERT_EQ(4, d);
    ASSERT_EQ(5, e);
    ASSERT_EQ(6, f);
    ASSERT_EQ(7, g);
    ASSERT_EQ(8, h);
    ASSERT_EQ(9, i);
    ASSERT_EQ(10, j);
    ASSERT_EQ(pillarPos[0], pillarPos_out[0]);
    ASSERT_EQ(pillarPos[1], pillarPos_out[1]);
    ASSERT_EQ(posOcc[0], posOcc_out[0]);
    ASSERT_EQ(posOcc[1], posOcc_out[1]);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_RoadWorksContainerExtendedLightBarSiren_Errors)
{
    int msgNotFoundSet = c2x::setDENMRoadWorksContainerExtendedLightBarSiren(0, 0, nullptr, 0);
    int msgNotFoundGet = c2x::getDENMRoadWorksContainerExtendedLightBarSiren(0, 0, nullptr, 0);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMRoadWorksContainerExtendedLightBarSiren(1, 1, nullptr, 0);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int rwcNull = c2x::getDENMRoadWorksContainerExtendedLightBarSiren(1, 1, nullptr, 0);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, rwcNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_RoadWorksContainerExtendedLightBarSiren)
{
    c2x::createDENM(1, 1);
    uint8_t a[2] = {7, 9};
    uint8_t a_out[2];
    int retSet = c2x::setDENMRoadWorksContainerExtendedLightBarSiren(1, 1, a, 2);
    int retGet = c2x::getDENMRoadWorksContainerExtendedLightBarSiren(1, 1, a_out, 2); 
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(a[0], a_out[0]);
    ASSERT_EQ(a[1], a_out[1]);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_RoadWorksContainerExtendedClosedLanes_Errors)
{
    int msgNotFoundSet = c2x::setDENMRoadWorksContainerExtendedClosedLanes(0, 0, 0, 0, 0);
    int msgNotFoundGet = c2x::getDENMRoadWorksContainerExtendedClosedLanes(0, 0, nullptr, nullptr, nullptr);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMRoadWorksContainerExtendedClosedLanes(1, 1, nullptr, nullptr, nullptr);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int rwcNull = c2x::getDENMRoadWorksContainerExtendedClosedLanes(1, 1, nullptr, nullptr, nullptr);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, rwcNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_RoadWorksContainerExtendedClosedLanes)
{
    c2x::createDENM(1, 1);
    int a, b, c;
    int retSet = c2x::setDENMRoadWorksContainerExtendedClosedLanes(1, 1, 2, 3, 4);
    int retGet = c2x::getDENMRoadWorksContainerExtendedClosedLanes(1, 1, &a, &b, &c); 
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(2, a);
    ASSERT_EQ(3, b);
    ASSERT_EQ(4, c);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_RoadWorksContainerExtendedRestriction_Errors)
{
    int msgNotFoundSet = c2x::setDENMRoadWorksContainerExtendedRestriction(0, 0, nullptr, 0);
    int msgNotFoundGet = c2x::getDENMRoadWorksContainerExtendedRestriction(0, 0, nullptr, 0);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMRoadWorksContainerExtendedRestriction(1, 1, nullptr, 0);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int rwcNull = c2x::getDENMRoadWorksContainerExtendedRestriction(1, 1, nullptr, 0);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, rwcNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_RoadWorksContainerExtendedRestriction)
{
    c2x::createDENM(1, 1);
    int a[2] = {1, 2};
    int a_out[2];
    int retSet = c2x::setDENMRoadWorksContainerExtendedRestriction(1, 1, a, 2);
    int retGet = c2x::getDENMRoadWorksContainerExtendedRestriction(1, 1, a_out, 2); 
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(a[0], a_out[0]);
    ASSERT_EQ(a[1], a_out[1]);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_RoadWorksContainerExtendedSpeedLimit_Errors)
{
    int msgNotFoundSet = c2x::setDENMRoadWorksContainerExtendedSpeedLimit(0, 0, 0);
    int msgNotFoundGet = c2x::getDENMRoadWorksContainerExtendedSpeedLimit(0, 0, nullptr);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMRoadWorksContainerExtendedSpeedLimit(1, 1, nullptr);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int rwcNull = c2x::getDENMRoadWorksContainerExtendedSpeedLimit(1, 1, nullptr);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, rwcNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_RoadWorksContainerExtendedSpeedLimit)
{
    c2x::createDENM(1, 1);
    int a;
    int retSet = c2x::setDENMRoadWorksContainerExtendedSpeedLimit(1, 1, 60);
    int retGet = c2x::getDENMRoadWorksContainerExtendedSpeedLimit(1, 1, &a); 
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(a, 60);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_RoadWorksContainerExtendedIncidentIndication_Errors)
{
    int msgNotFoundSet = c2x::setDENMRoadWorksContainerExtendedIncidentIndication(0, 0, 0, 0);
    int msgNotFoundGet = c2x::getDENMRoadWorksContainerExtendedIncidentIndication(0, 0, nullptr, nullptr);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMRoadWorksContainerExtendedIncidentIndication(1, 1, nullptr, nullptr);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int rwcNull = c2x::getDENMRoadWorksContainerExtendedIncidentIndication(1, 1, nullptr, nullptr);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, rwcNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_RoadWorksContainerExtendedIncidentIndication)
{
    c2x::createDENM(1, 1);
    int a, b;
    int retSet = c2x::setDENMRoadWorksContainerExtendedIncidentIndication(1, 1, 10, 11);
    int retGet = c2x::getDENMRoadWorksContainerExtendedIncidentIndication(1, 1, &a, &b); 
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(10, a);
    ASSERT_EQ(11, b);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_RoadWorksContainerExtendedRecommendedPath_Errors)
{
    int msgNotFoundSet = c2x::setDENMRoadWorksContainerExtendedRecommendedPath(0, 0, nullptr, 0);
    int msgNotFoundGet = c2x::getDENMRoadWorksContainerExtendedRecommendedPath(0, 0, nullptr, 0);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMRoadWorksContainerExtendedRecommendedPath(1, 1, nullptr, 0);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int rwcNull = c2x::getDENMRoadWorksContainerExtendedRecommendedPath(1, 1, nullptr, 0);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, rwcNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_RoadWorksContainerExtendedRecommendedPath)
{
    c2x::createDENM(1, 1);
    int refpos[14] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 10, 12, 13, 14};
    int refpos_out[14];
    int retSet = c2x::setDENMRoadWorksContainerExtendedRecommendedPath(1, 1, refpos, 14);
    int retGet = c2x::getDENMRoadWorksContainerExtendedRecommendedPath(1, 1, refpos_out, 14); 
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(refpos[0], refpos_out[0]);
    ASSERT_EQ(refpos[1], refpos_out[1]);
    ASSERT_EQ(refpos[2], refpos_out[2]);
    ASSERT_EQ(refpos[3], refpos_out[3]);
    ASSERT_EQ(refpos[4], refpos_out[4]);
    ASSERT_EQ(refpos[5], refpos_out[5]);
    ASSERT_EQ(refpos[6], refpos_out[6]);
    ASSERT_EQ(refpos[7], refpos_out[7]);
    ASSERT_EQ(refpos[8], refpos_out[8]);
    ASSERT_EQ(refpos[9], refpos_out[9]);
    ASSERT_EQ(refpos[10], refpos_out[10]);
    ASSERT_EQ(refpos[11], refpos_out[11]);
    ASSERT_EQ(refpos[12], refpos_out[12]);
    ASSERT_EQ(refpos[13], refpos_out[13]);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_RoadWorksContainerExtendedStartingPointSpeedLimit_Errors)
{
    int msgNotFoundSet = c2x::setDENMRoadWorksContainerExtendedStartingPointSpeedLimit(0, 0, 0, 0, 0);
    int msgNotFoundGet = c2x::getDENMRoadWorksContainerExtendedStartingPointSpeedLimit(0, 0, 0, 0, 0);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMRoadWorksContainerExtendedStartingPointSpeedLimit(1, 1, 0, 0, 0);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int rwcNull = c2x::getDENMRoadWorksContainerExtendedStartingPointSpeedLimit(1, 1, 0, 0, 0);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, rwcNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_RoadWorksContainerExtendedStartingPointSpeedLimit)
{
    c2x::createDENM(1, 1);
    int a, b, c;  
    int retSet = c2x::setDENMRoadWorksContainerExtendedStartingPointSpeedLimit(1, 1, 1, 2, 3);
    int retGet = c2x::getDENMRoadWorksContainerExtendedStartingPointSpeedLimit(1, 1, &a, &b, &c);
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(1, a);
    ASSERT_EQ(2, b);
    ASSERT_EQ(3, c);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_RoadWorksContainerExtendedTrafficFlowRule_Errors)
{
    int msgNotFoundSet = c2x::setDENMRoadWorksContainerExtendedTrafficFlowRule(0, 0, 0);
    int msgNotFoundGet = c2x::getDENMRoadWorksContainerExtendedTrafficFlowRule(0, 0, nullptr);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMRoadWorksContainerExtendedTrafficFlowRule(1, 1, nullptr);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int rwcNull = c2x::getDENMRoadWorksContainerExtendedTrafficFlowRule(1, 1, nullptr);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, rwcNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_RoadWorksContainerExtendedTrafficFlowRule)
{
    c2x::createDENM(1, 1);
    int a, b, c;  
    int retSet = c2x::setDENMRoadWorksContainerExtendedTrafficFlowRule(1, 1, 9);
    int retGet = c2x::getDENMRoadWorksContainerExtendedTrafficFlowRule(1, 1, &a);
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(9, a);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_RoadWorksContainerExtendedReferenceDenms_Errors)
{
    int msgNotFoundSet = c2x::setDENMRoadWorksContainerExtendedReferenceDenms(0, 0, nullptr, 0);
    int msgNotFoundGet = c2x::getDENMRoadWorksContainerExtendedReferenceDenms(0, 0, nullptr, 0);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMRoadWorksContainerExtendedReferenceDenms(1, 1, nullptr, 0);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int rwcNull = c2x::getDENMRoadWorksContainerExtendedReferenceDenms(1, 1, nullptr, 0);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, rwcNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_RoadWorksContainerExtendedReferenceDenms)
{
    c2x::createDENM(1, 1);
    int a[4] = {1, 1, 2, 2};
    int a_out[4];
    int retSet = c2x::setDENMRoadWorksContainerExtendedReferenceDenms(1, 1, a, 4);
    int retGet = c2x::getDENMRoadWorksContainerExtendedReferenceDenms(1, 1, a_out, 4);
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(a[0], a_out[0]);
    ASSERT_EQ(a[1], a_out[1]);
    ASSERT_EQ(a[3], a_out[3]);
    ASSERT_EQ(a[2], a_out[2]);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_StationaryVehicleContainerStationarySince_Errors)
{
    int msgNotFoundSet = c2x::setDENMStationaryVehicleContainerStationarySince(0, 0, 0);
    int msgNotFoundGet = c2x::getDENMStationaryVehicleContainerStationarySince(0, 0, nullptr);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMStationaryVehicleContainerStationarySince(1, 1, nullptr);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int svcNull = c2x::getDENMStationaryVehicleContainerStationarySince(1, 1, nullptr);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, svcNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_StationaryVehicleContainerStationarySince)
{
    c2x::createDENM(1, 1);
    int a;
    int retSet = c2x::setDENMStationaryVehicleContainerStationarySince(1, 1, 4);
    int retGet = c2x::getDENMStationaryVehicleContainerStationarySince(1, 1, &a);
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(4, a);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_StationaryVehicleContainerStationaryCause_Errors)
{
    int msgNotFoundSet = c2x::setDENMStationaryVehicleContainerStationaryCause(0, 0, 0, 0);
    int msgNotFoundGet = c2x::getDENMStationaryVehicleContainerStationaryCause(0, 0, nullptr, nullptr);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMStationaryVehicleContainerStationaryCause(1, 1, nullptr, nullptr);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int svcNull = c2x::getDENMStationaryVehicleContainerStationaryCause(1, 1, nullptr, nullptr);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, svcNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_StationaryVehicleContainerStationaryCause)
{
    c2x::createDENM(1, 1);
    int a, b;
    int retSet = c2x::setDENMStationaryVehicleContainerStationaryCause(1, 1, 2, 3);
    int retGet = c2x::getDENMStationaryVehicleContainerStationaryCause(1, 1, &a, &b);
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(2, a);
    ASSERT_EQ(3, b);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_StationaryVehicleContainerCarryingDangerousGoods_Errors)
{
    int msgNotFoundSet = c2x::setDENMStationaryVehicleContainerCarryingDangerousGoods(0, 0, 0, 0, 0, 0, 0);
    int msgNotFoundGet = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoods(0, 0, nullptr, nullptr, nullptr, nullptr, nullptr);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoods(1, 1, nullptr, nullptr, nullptr, nullptr, nullptr);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int svcNull = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoods(1, 1, nullptr, nullptr, nullptr, nullptr, nullptr);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, svcNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_StationaryVehicleContainerCarryingDangerousGoods)
{
    c2x::createDENM(1, 1);
    int a, b, c, d, e;
    int retSet = c2x::setDENMStationaryVehicleContainerCarryingDangerousGoods(1, 1, 1, 2, 3, 4, 5);
    int retGet = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoods(1, 1, &a, &b, &c, &d, &e);
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(1, a);    
    ASSERT_EQ(2, b);
    ASSERT_EQ(3, c);
    ASSERT_EQ(4, d);
    ASSERT_EQ(5, e);    
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_StationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode_Errors)
{
    int msgNotFoundSet = c2x::setDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode(0, 0, nullptr, 0);
    int msgNotFoundGet = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode(0, 0, nullptr, 0);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode(1, 1, nullptr, 0);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int svcNull = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode(1, 1, nullptr, 0);
    c2x::setDENMStationaryVehicleContainerCarryingDangerousGoods(1, 1, 0, 0, 0, 0, 0);
    int cdgNull = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode(1, 1, nullptr, 0);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, svcNull);
    ASSERT_EQ(ERR_NULL, cdgNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_StationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode)
{
    c2x::createDENM(1, 1);
    uint8_t a[2] = { 7, 8 }, a_out[2];
    int retSet = c2x::setDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode(1, 1, a, 2);
    int retGet = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode(1, 1, a_out, 2);
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(a[0], a_out[0]);
    ASSERT_EQ(a[1], a_out[1]);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_StationaryVehicleContainerCarryingDangerousGoodsPhoneNumber_Errors)
{
    int msgNotFoundSet = c2x::setDENMStationaryVehicleContainerCarryingDangerousGoodsPhoneNumber(0, 0, nullptr, 0);
    int msgNotFoundGet = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoodsPhoneNumber(0, 0, nullptr, 0);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoodsPhoneNumber(1, 1, nullptr, 0);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int svcNull = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoodsPhoneNumber(1, 1, nullptr, 0);
    c2x::setDENMStationaryVehicleContainerCarryingDangerousGoods(1, 1, 0, 0, 0, 0, 0);
    int cdgNull = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode(1, 1, nullptr, 0);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, svcNull);
    ASSERT_EQ(ERR_NULL, cdgNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_StationaryVehicleContainerCarryingDangerousGoodsPhoneNumber)
{
    c2x::createDENM(1, 1);
    uint8_t a[2] = { 3, 4 }, a_out[2];
    int retSet = c2x::setDENMStationaryVehicleContainerCarryingDangerousGoodsPhoneNumber(1, 1, a, 2);
    int retGet = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoodsPhoneNumber(1, 1, a_out, 2);
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(a[0], a_out[0]);
    ASSERT_EQ(a[1], a_out[1]);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_StationaryVehicleContainerCarryingDangerousGoodsCompanyName_Errors)
{
    int msgNotFoundSet = c2x::setDENMStationaryVehicleContainerCarryingDangerousGoodsCompanyName(0, 0, nullptr, 0);
    int msgNotFoundGet = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoodsCompanyName(0, 0, nullptr, 0);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoodsCompanyName(1, 1, nullptr, 0);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int svcNull = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoodsCompanyName(1, 1, nullptr, 0);
    c2x::setDENMStationaryVehicleContainerCarryingDangerousGoods(1, 1, 0, 0, 0, 0, 0);
    int cdgNull = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode(1, 1, nullptr, 0);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, svcNull);
    ASSERT_EQ(ERR_NULL, cdgNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_StationaryVehicleContainerCarryingDangerousGoodsCompanyName)
{
    c2x::createDENM(1, 1);
    uint8_t a[2] = { 3, 4 }, a_out[2];
    int retSet = c2x::setDENMStationaryVehicleContainerCarryingDangerousGoodsCompanyName(1, 1, a, 2);
    int retGet = c2x::getDENMStationaryVehicleContainerCarryingDangerousGoodsCompanyName(1, 1, a_out, 2);
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(a[0], a_out[0]);
    ASSERT_EQ(a[1], a_out[1]);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_StationaryVehicleContainerNumberOfOccupants_Errors)
{
    int msgNotFoundSet = c2x::setDENMStationaryVehicleContainerNumberOfOccupants(0, 0, 0);
    int msgNotFoundGet = c2x::getDENMStationaryVehicleContainerNumberOfOccupants(0, 0, nullptr);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMStationaryVehicleContainerNumberOfOccupants(1, 1, nullptr);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int svcNull = c2x::getDENMStationaryVehicleContainerNumberOfOccupants(1, 1, nullptr);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, svcNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_StationaryVehicleContainerNumberOfOccupants)
{
    c2x::createDENM(1, 1);
    int a;
    int retSet = c2x::setDENMStationaryVehicleContainerNumberOfOccupants(1, 1, 17);
    int retGet = c2x::getDENMStationaryVehicleContainerNumberOfOccupants(1, 1, &a);
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(17, a);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_StationaryVehicleContainerVehicleIdentification_Errors)
{
    int msgNotFoundSet = c2x::setDENMStationaryVehicleContainerVehicleIdentification(0, 0, nullptr, 0, nullptr, 0);
    int msgNotFoundGet = c2x::getDENMStationaryVehicleContainerVehicleIdentification(0, 0, nullptr, 0, nullptr, 0);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMStationaryVehicleContainerVehicleIdentification(1, 1, nullptr, 0, nullptr, 0);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int svcNull = c2x::getDENMStationaryVehicleContainerVehicleIdentification(1, 1, nullptr, 0, nullptr, 0);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, svcNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_StationaryVehicleContainerVehicleIdentification)
{
    c2x::createDENM(1, 1);
    uint8_t a[2] = {1, 2}, b[2] = {3, 4}, a_out[2], b_out[2];
    int retSet = c2x::setDENMStationaryVehicleContainerVehicleIdentification(1, 1, a, 2, b, 2);
    int retGet = c2x::getDENMStationaryVehicleContainerVehicleIdentification(1, 1, a_out, 2, b_out, 2);
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(a[0], a_out[0]);
    ASSERT_EQ(b[0], b_out[0]);
    ASSERT_EQ(a[1], a_out[1]);
    ASSERT_EQ(b[1], b_out[1]);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Check_StationaryVehicleContainerEnergyStorageType_Errors)
{
    int msgNotFoundSet = c2x::setDENMStationaryVehicleContainerEnergyStorageType(0, 0, 0);
    int msgNotFoundGet = c2x::getDENMStationaryVehicleContainerEnergyStorageType(0, 0, nullptr);
    c2x::createDENM(1, 1);
    int acNull = c2x::getDENMStationaryVehicleContainerEnergyStorageType(1, 1, nullptr);
    c2x::setDENMAlacarteContainer(1, 1, 0, 0, 0);
    int svcNull = c2x::getDENMStationaryVehicleContainerEnergyStorageType(1, 1, nullptr);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    ASSERT_EQ(ERR_NULL, acNull);
    ASSERT_EQ(ERR_NULL, svcNull);

    c2x::deleteDENM(1, 1);
}

TEST(DENM_AlacarteContainer, Set_And_Get_StationaryVehicleContainerEnergyStorageType)
{
    c2x::createDENM(1, 1);
    int a;
    int retSet = c2x::setDENMStationaryVehicleContainerEnergyStorageType(1, 1, 3);
    int retGet = c2x::getDENMStationaryVehicleContainerEnergyStorageType(1, 1, &a);
    ASSERT_EQ(0, retSet);
    ASSERT_EQ(0, retGet);
    ASSERT_EQ(a, 3);
    c2x::deleteDENM(1, 1);
}

TEST(DENM_ManagementContainer, Check_ManagementContainer_Errors)
{
    int msgNotFoundSet = c2x::setDENMManagementContainer(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    int msgNotFoundGet = c2x::getDENMManagementContainer(0, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
}

TEST(DENM_ManagementContainer, Set_And_Get_ManagementContainer)
{
    c2x::createDENM(1, 1);
    c2x::setDENMManagementContainer(1, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    int detTime, refTime, term, relDist, relTraffDir, valDur, transInt, statType;
    c2x::getDENMManagementContainer(1, 1, &detTime, &refTime, &term, &relDist, &relTraffDir, &valDur, &transInt, &statType);
    ASSERT_EQ(2, detTime);
    ASSERT_EQ(3, refTime);
    ASSERT_EQ(4, term);
    ASSERT_EQ(5, relDist);
    ASSERT_EQ(6, relTraffDir);
    ASSERT_EQ(7, valDur);
    ASSERT_EQ(8, transInt);
    ASSERT_EQ(9, statType);
}