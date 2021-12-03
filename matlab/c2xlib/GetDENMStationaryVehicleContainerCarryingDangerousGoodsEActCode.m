classdef GetDENMStationaryVehicleContainerCarryingDangerousGoodsEActCode < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        EmergencyActionCodeSize = int32(1);
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [EmergencyActionCode] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                EmergencyActionCode = uint8(zeros(obj.EmergencyActionCodeSize, 1));
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('getDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode', ...
                    StationID, SequenceNumber, coder.ref(EmergencyActionCode), length(EmergencyActionCode));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode';
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

    methods (Access = protected)
        function [EmergencyActionCode] = getOutputSizeImpl(obj)
            EmergencyActionCode = [obj.EmergencyActionCodeSize 1];
        end 
        
        function [EmergencyActionCode] = isOutputFixedSizeImpl(obj)
            EmergencyActionCode = true;
        end
        
        function [EmergencyActionCode] = getOutputDataTypeImpl(obj)
            EmergencyActionCode = 'uint8';
        end
        
        function [EmergencyActionCode] = isOutputComplexImpl(obj)
            EmergencyActionCode = false;
        end
    end
end