classdef GetDENMStationaryVehicleContainerCarryingDangerousGoodsCompName < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        CompanyNameSize = int32(16);
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [CompanyName] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                CompanyName = uint8(zeros(obj.CompanyNameSize, 1));
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('getDENMStationaryVehicleContainerCarryingDangerousGoodsCompanyName', ...
                    StationID, SequenceNumber, coder.ref(CompanyName), length(CompanyName));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMStationaryVehicleContainerCarryingDangerousGoodsCompanyName';
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
        function [CompanyName] = getOutputSizeImpl(obj)
            CompanyName = [obj.CompanyNameSize 1];
        end 
        
        function [CompanyName] = isOutputFixedSizeImpl(obj)
            CompanyName = true;
        end
        
        function [CompanyName] = getOutputDataTypeImpl(obj)
            CompanyName = 'uint8';
        end
        
        function [CompanyName] = isOutputComplexImpl(obj)
            CompanyName = false;
        end
    end
end