classdef GetCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [ProtectedZoneLatitude, ProtectedZoneLongitude, CenDsrcTollingZoneID] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                ProtectedZoneLatitude = int32(0);
                ProtectedZoneLongitude = int32(0);
                CenDsrcTollingZoneID = int32(0);
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('getCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone', StationID, ...
                    coder.wref(ProtectedZoneLatitude), coder.wref(ProtectedZoneLongitude), coder.wref(CenDsrcTollingZoneID));
            end            
        end
        
        function releaseImpl(~)            
        end        
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone';
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