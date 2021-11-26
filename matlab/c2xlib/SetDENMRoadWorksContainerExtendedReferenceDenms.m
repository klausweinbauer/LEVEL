classdef SetDENMRoadWorksContainerExtendedReferenceDenms < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        NumberActionIDs = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [] = stepImpl(obj, StationID, SequenceNumber, ...
            StationID_0, SequenceNumber_0, ...
            StationID_1, SequenceNumber_1, ...
            StationID_2, SequenceNumber_2, ...
            StationID_3, SequenceNumber_3, ...
            StationID_4, SequenceNumber_4, ...
            StationID_5, SequenceNumber_5, ...
            StationID_6, SequenceNumber_6, ...
            StationID_7, SequenceNumber_7, ...
            StationID_8, SequenceNumber_8, ...
            StationID_9, SequenceNumber_9)

            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                RefDENMs = [];
                if obj.NumberActionIDs > 0 
                    RefDENMs = [RefDENMs StationID_0 SequenceNumber_0];
                end
                if obj.NumberActionIDs > 1 
                    RefDENMs = [RefDENMs StationID_1 SequenceNumber_1];
                end
                if obj.NumberActionIDs > 2 
                    RefDENMs = [RefDENMs StationID_2 SequenceNumber_2];
                end
                if obj.NumberActionIDs > 3 
                    RefDENMs = [RefDENMs StationID_3 SequenceNumber_3];
                end
                if obj.NumberActionIDs > 4 
                    RefDENMs = [RefDENMs StationID_4 SequenceNumber_4];
                end
                if obj.NumberActionIDs > 5 
                    RefDENMs = [RefDENMs StationID_5 SequenceNumber_5];
                end
                if obj.NumberActionIDs > 6 
                    RefDENMs = [RefDENMs StationID_6 SequenceNumber_6];
                end
                if obj.NumberActionIDs > 7 
                    RefDENMs = [RefDENMs StationID_7 SequenceNumber_7];
                end
                if obj.NumberActionIDs > 8 
                    RefDENMs = [RefDENMs StationID_8 SequenceNumber_8];
                end
                if obj.NumberActionIDs > 9 
                    RefDENMs = [RefDENMs StationID_9 SequenceNumber_9];
                end
                coder.cinclude('c2xdenm.h');
                err = coder.ceval('setDENMRoadWorksContainerExtendedReferenceDenms', StationID, SequenceNumber, coder.ref(RefDENMs), length(RefDENMs));
                obj.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end

        function numIn = getNumInputsImpl(obj)
            numIn = obj.NumberActionIDs * 2 + 2;
        end

        function validatePropertiesImpl(obj)
            if ((obj.NumberActionIDs < 1 || obj.NumberActionIDs > 10))
                error("Number of Action IDs must be between 1 and 10.");
            end
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
            bName = 'SetDENMRoadWorksContainerExtendedReferenceDenms';
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
            [~, linkLibExt, execLibExt, ~] = buildContext.getStdLibInfo();

            % Parametrize library extension
            libName =  strcat('c2xdenm', linkLibExt);
            % Other linking parameters
            libPath = 'C:\Program Files\Polyspace\R2021a\extern\lib\win64\c2x';
            libPriority = '';
            libPreCompiled = true;
            libLinkOnly = true;

            % Linking command
            buildInfo.addLinkObjects(libName,libPath,libPriority,libPreCompiled,libLinkOnly);
            buildInfo.addIncludePaths(libPath);
            buildInfo.addIncludeFiles('c2xcommon.h');
        end
    end
end