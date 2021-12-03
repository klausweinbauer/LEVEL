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
                coder.cinclude('c2xcam.h');
                coder.ceval('getCAMBasicVehicleContainerLowFrequency', StationID, coder.ref(VehicleRole), ...
                    coder.ref(ExteriorLights), length(ExteriorLights), coder.ref(ActualExteriorLightsSize));
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