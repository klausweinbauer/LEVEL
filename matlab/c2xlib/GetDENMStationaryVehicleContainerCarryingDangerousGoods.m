classdef GetDENMStationaryVehicleContainerCarryingDangerousGoods < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [DangerousGoodsType, UnNumber, ElevatedTemperature, TunnelsRestricted, LimitedQuantity] ...
            = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                DangerousGoodsType = int32(0);
                UnNumber = int32(0);
                ElevatedTemperature = int32(0);
                TunnelsRestricted = int32(0);
                LimitedQuantity = int32(0);
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('getDENMStationaryVehicleContainerCarryingDangerousGoods', StationID, SequenceNumber, ...
                    coder.ref(DangerousGoodsType), coder.ref(UnNumber), coder.ref(ElevatedTemperature), ...
                    coder.ref(TunnelsRestricted), coder.ref(LimitedQuantity));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMStationaryVehicleContainerCarryingDangerousGoods';
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
end