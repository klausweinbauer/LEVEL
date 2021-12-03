classdef GetCAMBasicVehicleContainerLowFrequency < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        ExteriorLightsSize = 1;  
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [VehicleRole, ExteriorLights, ActualExteriorLightsSize] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                VehicleRole = int32(0);
                ExteriorLights = uint8(zeros(obj.ExteriorLightsSize, 1));
                ActualExteriorLightsSize = int32(0);
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('getCAMBasicVehicleContainerLowFrequency', StationID, coder.ref(VehicleRole), ...
                    coder.ref(ExteriorLights), length(ExteriorLights), coder.ref(ActualExteriorLightsSize));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMBasicVehicleContainerLowFrequency';
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
    methods (Access = protected)
        function [VehicleRole, ExteriorLights, ActualExteriorLightsSize] = getOutputSizeImpl(obj)
            VehicleRole = [1, 1];
            ExteriorLights = [obj.ExteriorLightsSize, 1];
            ActualExteriorLightsSize = [1, 1];
        end 
        
        function [VehicleRole, ExteriorLights, ActualExteriorLightsSize] = isOutputFixedSizeImpl(obj)
            VehicleRole = true;
            ExteriorLights = true;
            ActualExteriorLightsSize = true;
        end
        
        function [VehicleRole, ExteriorLights, ActualExteriorLightsSize] = getOutputDataTypeImpl(obj)
            VehicleRole = 'int32';
            ExteriorLights = 'uint8';
            ActualExteriorLightsSize = 'int32';
        end
        
        function [VehicleRole, ExteriorLights, ActualExteriorLightsSize] = isOutputComplexImpl(obj)
            VehicleRole = false;
            ExteriorLights = false;
            ActualExteriorLightsSize = false;
        end
    end
end