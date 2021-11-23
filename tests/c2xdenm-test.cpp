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