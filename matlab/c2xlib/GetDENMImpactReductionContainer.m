classdef GetDENMImpactReductionContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        PositionOfPillarsSize = 1;
        PositionOfOccupantsSize = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [HeightLonCarrLeft, HeightLonCarrRight, PosLonCarrLeft, ...
            PosLonCarrRight, PositionOfPillars, PosCentMass, WheelBaseVehicle, TurningRadius, PosFrontAx, ...
            PositionOfOccupants, VehicleMass, RequestResponseIndication] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                HeightLonCarrLeft = int32(0); 
                HeightLonCarrRight = int32(0); 
                PosLonCarrLeft = int32(0); 
                PosLonCarrRight = int32(0); 
                PositionOfPillars = int32(zeros(obj.PositionOfPillarsSize, 1)); 
                PosCentMass = int32(0); 
                WheelBaseVehicle = int32(0); 
                TurningRadius = int32(0); 
                PosFrontAx = int32(0); 
                PositionOfOccupants = uint8(zeros(obj.PositionOfOccupantsSize, 1)); 
                VehicleMass = int32(0); 
                RequestResponseIndication = int32(0);
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('getDENMImpactReductionContainer', StationID, SequenceNumber, ...
                    coder.ref(HeightLonCarrLeft), ...
                    coder.ref(HeightLonCarrRight), ...
                    coder.ref(PosLonCarrLeft), ...
                    coder.ref(PosLonCarrRight), ...
                    coder.ref(PositionOfPillars), obj.PositionOfPillarsSize, ...
                    coder.ref(PosCentMass), ...
                    coder.ref(WheelBaseVehicle), ...
                    coder.ref(TurningRadius), ...
                    coder.ref(PosFrontAx), ...
                    coder.ref(PositionOfOccupants), obj.PositionOfOccupantsSize,  ...
                    coder.ref(VehicleMass), ...
                    coder.ref(RequestResponseIndication));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMImpactReductionContainer';
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
        function [HeightLonCarrLeft, HeightLonCarrRight, PosLonCarrLeft, ...
            PosLonCarrRight, PositionOfPillars, PosCentMass, WheelBaseVehicle, TurningRadius, PosFrontAx, ...
            PositionOfOccupants, VehicleMass, RequestResponseIndication] = getOutputSizeImpl(obj)

            HeightLonCarrLeft = [1 1];
            HeightLonCarrRight = [1 1];
            PosLonCarrLeft = [1 1];
            PosLonCarrRight = [1 1];
            PositionOfPillars = [obj.PositionOfPillarsSize 1]; 
            PosCentMass = [1 1];
            WheelBaseVehicle = [1 1];
            TurningRadius = [1 1];
            PosFrontAx = [1 1];
            PositionOfOccupants = [obj.PositionOfOccupantsSize 1]; 
            VehicleMass = [1 1];
            RequestResponseIndication = [1 1];
        end 
        
        function [HeightLonCarrLeft, HeightLonCarrRight, PosLonCarrLeft, ...
            PosLonCarrRight, PositionOfPillars, PosCentMass, WheelBaseVehicle, TurningRadius, PosFrontAx, ...
            PositionOfOccupants, VehicleMass, RequestResponseIndication] = isOutputFixedSizeImpl(obj)

            HeightLonCarrLeft = true;
            HeightLonCarrRight = true;
            PosLonCarrLeft = true;
            PosLonCarrRight = true;
            PositionOfPillars = true;
            PosCentMass = true;
            WheelBaseVehicle = true;
            TurningRadius = true;
            PosFrontAx = true;
            PositionOfOccupants = true;
            VehicleMass = true;
            RequestResponseIndication = true;
        end
        
        function [HeightLonCarrLeft, HeightLonCarrRight, PosLonCarrLeft, ...
            PosLonCarrRight, PositionOfPillars, PosCentMass, WheelBaseVehicle, TurningRadius, PosFrontAx, ...
            PositionOfOccupants, VehicleMass, RequestResponseIndication] = getOutputDataTypeImpl(obj)
            
            HeightLonCarrLeft = 'int32';
            HeightLonCarrRight = 'int32';
            PosLonCarrLeft = 'int32';
            PosLonCarrRight = 'int32';
            PositionOfPillars = 'int32';
            PosCentMass = 'int32';
            WheelBaseVehicle = 'int32';
            TurningRadius = 'int32';
            PosFrontAx = 'int32';
            PositionOfOccupants = 'uint8';
            VehicleMass = 'int32';
            RequestResponseIndication = 'int32';
        end
        
        function [HeightLonCarrLeft, HeightLonCarrRight, PosLonCarrLeft, ...
            PosLonCarrRight, PositionOfPillars, PosCentMass, WheelBaseVehicle, TurningRadius, PosFrontAx, ...
            PositionOfOccupants, VehicleMass, RequestResponseIndication] = isOutputComplexImpl(obj)
            
            HeightLonCarrLeft = false;
            HeightLonCarrRight = false;
            PosLonCarrLeft = false;
            PosLonCarrRight = false;
            PositionOfPillars = false;
            PosCentMass = false;
            WheelBaseVehicle = false;
            TurningRadius = false;
            PosFrontAx = false;
            PositionOfOccupants = false;
            VehicleMass = false;
            RequestResponseIndication = false;
        end
    end
end