classdef SetDENMRoadWorksContainerExtendedReferenceDenms < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        NumberActionIDs = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [] = stepImpl(obj, StationID, SequenceNumber, ...
            StationID_0, SequenceNumber_0, ...
            StationID_1, SequenceNumber_1, ...
            StationID_2, SequenceNumber_2, ...
            StationID_3, SequenceNumber_3, ...
            StationID_4, SequenceNumber_4, ...
            StationID_5, SequenceNumber_5, ...
            StationID_6, SequenceNumber_6, ...
            StationID_7, SequenceNumber_7, ...
            StationID_8, SequenceNumber_8, ...
            StationID_9, SequenceNumber_9)

            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                RefDENMs = [];
                if obj.NumberActionIDs > 0 
                    RefDENMs = [RefDENMs int32(StationID_0) int32(SequenceNumber_0)];
                end
                if obj.NumberActionIDs > 1 
                    RefDENMs = [RefDENMs int32(StationID_1) int32(SequenceNumber_1)];
                end
                if obj.NumberActionIDs > 2 
                    RefDENMs = [RefDENMs int32(StationID_2) int32(SequenceNumber_2)];
                end
                if obj.NumberActionIDs > 3 
                    RefDENMs = [RefDENMs int32(StationID_3) int32(SequenceNumber_3)];
                end
                if obj.NumberActionIDs > 4 
                    RefDENMs = [RefDENMs int32(StationID_4) int32(SequenceNumber_4)];
                end
                if obj.NumberActionIDs > 5 
                    RefDENMs = [RefDENMs int32(StationID_5) int32(SequenceNumber_5)];
                end
                if obj.NumberActionIDs > 6 
                    RefDENMs = [RefDENMs int32(StationID_6) int32(SequenceNumber_6)];
                end
                if obj.NumberActionIDs > 7 
                    RefDENMs = [RefDENMs int32(StationID_7) int32(SequenceNumber_7)];
                end
                if obj.NumberActionIDs > 8 
                    RefDENMs = [RefDENMs int32(StationID_8) int32(SequenceNumber_8)];
                end
                if obj.NumberActionIDs > 9 
                    RefDENMs = [RefDENMs int32(StationID_9) int32(SequenceNumber_9)];
                end
                coder.cinclude(LibConfig.getDENMHeader());
                err = coder.ceval('setDENMRoadWorksContainerExtendedReferenceDenms', StationID, SequenceNumber, coder.ref(RefDENMs), length(RefDENMs));
                LibConfig.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end

        function numIn = getNumInputsImpl(obj)
            numIn = obj.NumberActionIDs * 2 + 2;
        end

        function validatePropertiesImpl(obj)
            if ((obj.NumberActionIDs < 1 || obj.NumberActionIDs > 10))
                error("Number of Action IDs must be between 1 and 10.");
            end
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'SetDENMRoadWorksContainerExtendedReferenceDenms';
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