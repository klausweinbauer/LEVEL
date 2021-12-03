classdef GetDENMStationaryVehicleContainerCarryingDangerousGoodsPhoneNbr < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        PhoneNumberSize = int32(16);
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [PhoneNumber] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                PhoneNumber = uint8(zeros(obj.PhoneNumberSize, 1));
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('getDENMStationaryVehicleContainerCarryingDangerousGoodsPhoneNumber', ...
                    StationID, SequenceNumber, coder.ref(PhoneNumber), length(PhoneNumber));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMStationaryVehicleContainerCarryingDangerousGoodsPhoneNumber';
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
        function [PhoneNumber] = getOutputSizeImpl(obj)
            PhoneNumber = [obj.PhoneNumberSize 1];
        end 
        
        function [PhoneNumber] = isOutputFixedSizeImpl(obj)
            PhoneNumber = true;
        end
        
        function [PhoneNumber] = getOutputDataTypeImpl(obj)
            PhoneNumber = 'uint8';
        end
        
        function [PhoneNumber] = isOutputComplexImpl(obj)
            PhoneNumber = false;
        end
    end
end