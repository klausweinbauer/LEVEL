classdef SetCAMRSUContainerHighFrequency < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        ProtectedCommunicationZoneNumber = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [] = stepImpl(obj, StationID, Zone1, Zone2, Zone3, Zone4, Zone5, Zone6, Zone7, Zone8, Zone9, Zone10) 
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                coder.cinclude('c2xcam.h');
                err = coder.ceval('clearCAMRSUContainerHighFrequencyProtectedCommunicationZones', StationID);
                if obj.ProtectedCommunicationZoneNumber > 0
                    if length(Zone1) ~= 6
                        error('Input Zone1 must be of length 6. You could use a ProtectedCommunicationZone Block as input.');
                    end
                    coder.ceval('addCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, Zone1(1), Zone1(2), Zone1(3), Zone1(4), Zone1(5), Zone1(6));
                end
                if obj.ProtectedCommunicationZoneNumber > 1
                    if length(Zone2) ~= 6
                        error('Input Zone2 must be of length 6. You could use a ProtectedCommunicationZone Block as input.');
                    end
                    coder.ceval('addCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, Zone2(1), Zone2(2), Zone2(3), Zone2(4), Zone2(5), Zone2(6));
                end
                if obj.ProtectedCommunicationZoneNumber > 2
                    if length(Zone3) ~= 6
                        error('Input Zone3 must be of length 6. You could use a ProtectedCommunicationZone Block as input.');
                    end
                    coder.ceval('addCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, Zone3(1), Zone3(2), Zone3(3), Zone3(4), Zone3(5), Zone3(6));
                end
                if obj.ProtectedCommunicationZoneNumber > 3
                    if length(Zone4) ~= 6
                        error('Input Zone4 must be of length 6. You could use a ProtectedCommunicationZone Block as input.');
                    end
                    coder.ceval('addCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, Zone4(1), Zone4(2), Zone4(3), Zone4(4), Zone4(5), Zone4(6));
                end
                if obj.ProtectedCommunicationZoneNumber > 4
                    if length(Zone5) ~= 6
                        error('Input Zone5 must be of length 6. You could use a ProtectedCommunicationZone Block as input.');
                    end
                    coder.ceval('addCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, Zone5(1), Zone5(2), Zone5(3), Zone5(4), Zone5(5), Zone5(6));
                end
                if obj.ProtectedCommunicationZoneNumber > 5
                    if length(Zone6) ~= 6
                        error('Input Zone6 must be of length 6. You could use a ProtectedCommunicationZone Block as input.');
                    end
                    coder.ceval('addCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, Zone6(1), Zone6(2), Zone6(3), Zone6(4), Zone6(5), Zone6(6));
                end
                if obj.ProtectedCommunicationZoneNumber > 6
                    if length(Zone7) ~= 6
                        error('Input Zone7 must be of length 6. You could use a ProtectedCommunicationZone Block as input.');
                    end
                    coder.ceval('addCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, Zone7(1), Zone7(2), Zone7(3), Zone7(4), Zone7(5), Zone7(6));
                end
                if obj.ProtectedCommunicationZoneNumber > 7
                    if length(Zone8) ~= 6
                        error('Input Zone8 must be of length 6. You could use a ProtectedCommunicationZone Block as input.');
                    end
                    coder.ceval('addCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, Zone8(1), Zone8(2), Zone8(3), Zone8(4), Zone8(5), Zone8(6));
                end
                if obj.ProtectedCommunicationZoneNumber > 8
                    if length(Zone9) ~= 6
                        error('Input Zone9 must be of length 6. You could use a ProtectedCommunicationZone Block as input.');
                    end
                    coder.ceval('addCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, Zone9(1), Zone9(2), Zone9(3), Zone9(4), Zone9(5), Zone9(6));
                end
                if obj.ProtectedCommunicationZoneNumber > 9
                    if length(Zone10) ~= 6
                        error('Input Zone10 must be of length 6. You could use a ProtectedCommunicationZone Block as input.');
                    end
                    coder.ceval('addCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, Zone10(1), Zone10(2), Zone10(3), Zone10(4), Zone10(5), Zone10(6));
                end                
                obj.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end

        function numIn = getNumInputsImpl(obj)
            numIn = obj.ProtectedCommunicationZoneNumber + 1;
        end

        function validatePropertiesImpl(obj)
            if ((obj.ProtectedCommunicationZoneNumber < 1 || obj.ProtectedCommunicationZoneNumber > 10))
                error("Number of Zones must be between 1 and 10.");
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
            bName = 'SetCAMRSUContainerHighFrequency';
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
            LibConfig.updateBuildInfoCAM(buildInfo, buildContext);
        end
    end
end