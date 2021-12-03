classdef SetDENMImpactReductionContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [] = stepImpl(obj, StationID, SequenceNumber, HeightLonCarrLeft, HeightLonCarrRight, PosLonCarrLeft, ...
            PosLonCarrRight, PositionOfPillars, PosCentMass, WheelBaseVehicle, TurningRadius, PosFrontAx, ...
            PositionOfOccupants, VehicleMass, RequestResponseIndication)
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                TmpPositionOfPillars = int32(PositionOfPillars);
                TmpPositionOfOccupants = uint8(PositionOfOccupants);
                coder.cinclude(LibConfig.getDENMHeader());
                err = coder.ceval('setDENMImpactReductionContainer', StationID, SequenceNumber, int32(HeightLonCarrLeft), ...
                    int32(HeightLonCarrRight), int32(PosLonCarrLeft), int32(PosLonCarrRight), coder.ref(TmpPositionOfPillars), length(TmpPositionOfPillars), ...
                    int32(PosCentMass), int32(WheelBaseVehicle), int32(TurningRadius), int32(PosFrontAx), ...
                    coder.ref(TmpPositionOfOccupants), length(TmpPositionOfOccupants), int32(VehicleMass), int32(RequestResponseIndication));
                LibConfig.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'SetDENMImpactReductionContainer';
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