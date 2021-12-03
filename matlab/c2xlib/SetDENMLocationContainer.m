classdef SetDENMLocationContainer < matlab.System & coder.ExternalDependency
    % Traces T0 - T9 are only initialized once. Value changes have no effect. 

    properties (Nontunable)
        NumberTraces = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
        IsInitialized = false;
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [] = stepImpl(obj, StationID, SequenceNumber, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)
            if coder.target('Rtw') || coder.target('Sfun')                
                coder.cinclude(LibConfig.getDENMHeader());
                if ~obj.IsInitialized
                    obj.IsInitialized = true;
                    coder.ceval('clearDENMLocationContainerTraces', StationID, SequenceNumber);               
                    if obj.NumberTraces > 0
                        T0Buffer = int32(T0(:));
                        coder.ceval('addDENMLocationContainerTrace', StationID, SequenceNumber, coder.ref(T0Buffer), length(T0Buffer));
                    end
                    if obj.NumberTraces > 1
                        T1Buffer = int32(T1(:));
                        coder.ceval('addDENMLocationContainerTrace', StationID, SequenceNumber, coder.ref(T1Buffer), length(T1Buffer));
                    end
                    if obj.NumberTraces > 2
                        T2Buffer = int32(T2(:));
                        coder.ceval('addDENMLocationContainerTrace', StationID, SequenceNumber, coder.ref(T2Buffer), length(T2Buffer));
                    end
                    if obj.NumberTraces > 3
                        T3Buffer = int32(T3(:));
                        coder.ceval('addDENMLocationContainerTrace', StationID, SequenceNumber, coder.ref(T3Buffer), length(T3Buffer));
                    end
                    if obj.NumberTraces > 4
                        T4Buffer = int32(T4(:));
                        coder.ceval('addDENMLocationContainerTrace', StationID, SequenceNumber, coder.ref(T4Buffer), length(T4Buffer));
                    end
                    if obj.NumberTraces > 5
                        T5Buffer = int32(T5(:));
                        coder.ceval('addDENMLocationContainerTrace', StationID, SequenceNumber, coder.ref(T5Buffer), length(T5Buffer));
                    end
                    if obj.NumberTraces > 6
                        T6Buffer = int32(T6(:));
                        coder.ceval('addDENMLocationContainerTrace', StationID, SequenceNumber, coder.ref(T6Buffer), length(T6Buffer));
                    end
                    if obj.NumberTraces > 7
                        T7Buffer = int32(T7(:));
                        coder.ceval('addDENMLocationContainerTrace', StationID, SequenceNumber, coder.ref(T7Buffer), length(T7Buffer));
                    end
                    if obj.NumberTraces > 8
                        T8Buffer = int32(T8(:));
                        coder.ceval('addDENMLocationContainerTrace', StationID, SequenceNumber, coder.ref(T8Buffer), length(T8Buffer));
                    end
                    if obj.NumberTraces > 9
                        T9Buffer = int32(T9(:));
                        coder.ceval('addDENMLocationContainerTrace', StationID, SequenceNumber, coder.ref(T9Buffer), length(T9Buffer));
                    end
                end
            end          
        end
        
        function releaseImpl(~)            
        end

        function validatePropertiesImpl(obj)
            if ((obj.NumberTraces < 1 || obj.NumberTraces > 10))
                error("Number of traces must be between 1 and 10.");
            end
        end
        
        function numIn = getNumInputsImpl(obj)
            numIn = obj.NumberTraces + 2;
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'SetDENMLocationContainer';
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