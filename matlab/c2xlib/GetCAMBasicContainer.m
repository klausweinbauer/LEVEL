classdef GetCAMBasicContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [StationType, Latitude, Longitude, ConfidenceMajor, ConfidenceMinor, ...
            ConfidenceMajorOrientation, AltitudeValue, AltitudeConfidence] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                StationType = int32(0);
                Latitude = int32(0);
                Longitude = int32(0);
                ConfidenceMajor = int32(0);
                ConfidenceMinor = int32(0);
                ConfidenceMajorOrientation = int32(0);
                AltitudeValue = int32(0);
                AltitudeConfidence = int32(0);
                coder.cinclude('c2xcam.h');
                coder.ceval('getCAMBasicContainer', StationID, coder.wref(StationType), ...
                    coder.wref(Latitude), coder.wref(Longitude), coder.wref(ConfidenceMajor), ...
                    coder.wref(ConfidenceMinor), coder.wref(ConfidenceMajorOrientation), ...
                    coder.wref(AltitudeValue), coder.wref(AltitudeConfidence));
            end            
        end
        
        function releaseImpl(~)            
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
            bName = 'GetCAMBasicContainer';
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