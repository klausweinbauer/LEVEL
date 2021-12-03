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
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('getCAMBasicContainer', StationID, coder.wref(StationType), ...
                    coder.wref(Latitude), coder.wref(Longitude), coder.wref(ConfidenceMajor), ...
                    coder.wref(ConfidenceMinor), coder.wref(ConfidenceMajorOrientation), ...
                    coder.wref(AltitudeValue), coder.wref(AltitudeConfidence));
            end            
        end
        
        function releaseImpl(~)            
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