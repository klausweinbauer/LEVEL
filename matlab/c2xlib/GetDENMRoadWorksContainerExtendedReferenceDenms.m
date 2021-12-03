classdef GetDENMRoadWorksContainerExtendedReferenceDenms < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        ActionIDsSize = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [...
            StationID_0, SequenceNumber_0, ...
            StationID_1, SequenceNumber_1, ...
            StationID_2, SequenceNumber_2, ...
            StationID_3, SequenceNumber_3, ...
            StationID_4, SequenceNumber_4, ...
            StationID_5, SequenceNumber_5, ...
            StationID_6, SequenceNumber_6, ...
            StationID_7, SequenceNumber_7, ...
            StationID_8, SequenceNumber_8, ...
            StationID_9, SequenceNumber_9] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun')
                ActionIDs = int32(zeros(obj.ActionIDsSize * 2, 1));
                coder.cinclude('c2xdenm.h');
                coder.ceval('getDENMRoadWorksContainerExtendedReferenceDenms', StationID, SequenceNumber, ...
                    coder.ref(ActionIDs), length(ActionIDs));
                if obj.ActionIDsSize > 0
                    StationID_0 = ActionIDs(1);
                    SequenceNumber_0 = ActionIDs(2);
                end
                if obj.ActionIDsSize > 1
                    StationID_1 = ActionIDs(3);
                    SequenceNumber_1 = ActionIDs(4);
                end
                if obj.ActionIDsSize > 2
                    StationID_2 = ActionIDs(5);
                    SequenceNumber_2 = ActionIDs(6);
                end
                if obj.ActionIDsSize > 3
                    StationID_3 = ActionIDs(7);
                    SequenceNumber_3 = ActionIDs(8);
                end
                if obj.ActionIDsSize > 4
                    StationID_4 = ActionIDs(9);
                    SequenceNumber_4 = ActionIDs(10);
                end
                if obj.ActionIDsSize > 5
                    StationID_5 = ActionIDs(11);
                    SequenceNumber_5 = ActionIDs(12);
                end
                if obj.ActionIDsSize > 6
                    StationID_6 = ActionIDs(13);
                    SequenceNumber_6 = ActionIDs(14);
                end
                if obj.ActionIDsSize > 7
                    StationID_7 = ActionIDs(15);
                    SequenceNumber_7 = ActionIDs(16);
                end
                if obj.ActionIDsSize > 8
                    StationID_8 = ActionIDs(17);
                    SequenceNumber_8 = ActionIDs(18);
                end
                if obj.ActionIDsSize > 9
                    StationID_9 = ActionIDs(19);
                    SequenceNumber_9 = ActionIDs(20);
                end
            end            
        end
        
        function releaseImpl(~)            
        end

        function num = getNumOutputsImpl(obj)
            num = obj.ActionIDsSize * 2;
        end

        function printErrorCode(~, err)
            if (err == -22)
                error('ERR_CAM_ALREADY_EXISTS')
            elseif (err == -21)
                error('ERR_LOW_FREQ_CONTAINER_TYPE_BASIC_VEHICLE')
            elseif (err == -20)
                error('ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE')
            elseif (err == -40)
                error('ERR_DENM_ALREADY_EXISTS')
            elseif (err == -1)
                error('ERR_MSG_NOT_FOUND')
            elseif (err == -9)
                error('ERR_ARG_NULL')
            elseif (err == -8)
                error('ERR_TRANSMITTER_START')
            elseif (err == -7)
                error('ERR_RECEIVER_START')
            elseif (err == -6)
                error('ERR_DECODE')
            elseif (err == -5)
                error('ERR_ENCODE')
            elseif (err == -4)
                error('ERR_BUFFER_OVERFLOW')
            elseif (err == -3)
                error('ERR_NULL')
            elseif (err == -2)
                error('ERR_ALLOC_FAILED')
            end
        end

    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMRoadWorksContainerExtendedReferenceDenms';
        end
        
        function supported = isSupportedContext(buildContext)
            myTarget = {'rtw'};
            if  buildContext.isCodeGenTarget(myTarget)
                supported = true;
            else
                error('API only supported for mex, lib, exe, dll');
            end
        end
        
        function updateBuildInfo(buildInfo, buildContext)
            LibConfig.updateBuildInfoDENM(buildInfo, buildContext);
        end
    end
end