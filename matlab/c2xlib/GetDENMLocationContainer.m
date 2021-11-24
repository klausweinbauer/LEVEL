classdef GetDENMLocationContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)   
        NumberTraces = 1;
        TraceLength = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [T0, T1, T2, T3, T4, T5, T6, T7, T8, T9] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                T0Buffer = int32(zeros(obj.TraceLength * 4, 1));
                T1Buffer = int32(zeros(obj.TraceLength * 4, 1));
                T2Buffer = int32(zeros(obj.TraceLength * 4, 1));
                T3Buffer = int32(zeros(obj.TraceLength * 4, 1));
                T4Buffer = int32(zeros(obj.TraceLength * 4, 1));
                T5Buffer = int32(zeros(obj.TraceLength * 4, 1));
                T6Buffer = int32(zeros(obj.TraceLength * 4, 1));
                T7Buffer = int32(zeros(obj.TraceLength * 4, 1));
                T8Buffer = int32(zeros(obj.TraceLength * 4, 1));                
                T9Buffer = int32(zeros(obj.TraceLength * 4, 1));
                T10Buffer = int32(zeros(obj.TraceLength * 4, 1));
                ActualSize = int32(0);
                coder.cinclude('c2xdenm.h');
                if obj.NumberTraces > 0
                    coder.ceval('getDENMLocationContainerTrace', StationID, SequenceNumber, 0, coder.ref(T0Buffer), length(T0Buffer), coder.ref(ActualSize));
                    T0 = int32(reshape(T0Buffer, obj.TraceLength, 4));
                end
                if obj.NumberTraces > 1
                    coder.ceval('getDENMLocationContainerTrace', StationID, SequenceNumber, 1, coder.ref(T1Buffer), length(T1Buffer), coder.ref(ActualSize));
                    T1 = reshape(T1Buffer, obj.TraceLength, 4);
                end
                if obj.NumberTraces > 2
                    coder.ceval('getDENMLocationContainerTrace', StationID, SequenceNumber, 2, coder.ref(T2Buffer), length(T2Buffer), coder.ref(ActualSize));
                    T2 = reshape(T2Buffer, obj.TraceLength, 4);
                end
                if obj.NumberTraces > 3
                    coder.ceval('getDENMLocationContainerTrace', StationID, SequenceNumber, 3, coder.ref(T3Buffer), length(T3Buffer), coder.ref(ActualSize));
                    T3 = reshape(T3Buffer, obj.TraceLength, 4);
                end
                if obj.NumberTraces > 4
                    coder.ceval('getDENMLocationContainerTrace', StationID, SequenceNumber, 4, coder.ref(T4Buffer), length(T4Buffer), coder.ref(ActualSize));
                    T4 = reshape(T4Buffer, obj.TraceLength, 4);
                end
                if obj.NumberTraces > 5
                    coder.ceval('getDENMLocationContainerTrace', StationID, SequenceNumber, 5, coder.ref(T5Buffer), length(T5Buffer), coder.ref(ActualSize));
                    T5 = reshape(T5Buffer, obj.TraceLength, 4);
                end
                if obj.NumberTraces > 6
                    coder.ceval('getDENMLocationContainerTrace', StationID, SequenceNumber, 6, coder.ref(T6Buffer), length(T6Buffer), coder.ref(ActualSize));
                    T6 = reshape(T6Buffer, obj.TraceLength, 4);
                end
                if obj.NumberTraces > 7
                    coder.ceval('getDENMLocationContainerTrace', StationID, SequenceNumber, 7, coder.ref(T7Buffer), length(T7Buffer), coder.ref(ActualSize));
                    T7 = reshape(T7Buffer, obj.TraceLength, 4);
                end
                if obj.NumberTraces > 8
                    coder.ceval('getDENMLocationContainerTrace', StationID, SequenceNumber, 8, coder.ref(T8Buffer), length(T8Buffer), coder.ref(ActualSize));
                    T8 = reshape(T8Buffer, obj.TraceLength, 4);
                end
                if obj.NumberTraces > 9
                    coder.ceval('getDENMLocationContainerTrace', StationID, SequenceNumber, 9, coder.ref(T9Buffer), length(T9Buffer), coder.ref(ActualSize));
                    T9 = reshape(T9Buffer, obj.TraceLength, 4);
                end
                
            end 
        end
        
        function releaseImpl(~)            
        end

        function num = getNumOutputsImpl(obj)
            num = obj.NumberTraces;
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMLocationContainer';
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
            [~, linkLibExt, execLibExt, ~] = buildContext.getStdLibInfo();

            % Parametrize library extension
            libName =  strcat('c2xdenm', linkLibExt);
            % Other linking parameters
            libPath = 'C:\Program Files\Polyspace\R2021a\extern\lib\win64\c2x';
            libPriority = '';
            libPreCompiled = true;
            libLinkOnly = true;

            % Linking command
            buildInfo.addLinkObjects(libName,libPath,libPriority,libPreCompiled,libLinkOnly);
            buildInfo.addIncludePaths(libPath);
            buildInfo.addIncludeFiles('c2xcommon.h');
        end
    end

    methods (Access = protected)
        function [T0, T1, T2, T3, T4, T5, T6, T7, T8, T9] = getOutputSizeImpl(obj)
            T0 = [obj.TraceLength, 4];
            T1 = [obj.TraceLength, 4];
            T2 = [obj.TraceLength, 4];
            T3 = [obj.TraceLength, 4];
            T4 = [obj.TraceLength, 4];
            T5 = [obj.TraceLength, 4];
            T6 = [obj.TraceLength, 4];
            T7 = [obj.TraceLength, 4];
            T8 = [obj.TraceLength, 4];
            T9 = [obj.TraceLength, 4];
        end 
        
        function [T0, T1, T2, T3, T4, T5, T6, T7, T8, T9] = isOutputFixedSizeImpl(obj)
            T0 = true;
            T1 = true;
            T2 = true;
            T3 = true;
            T4 = true;
            T5 = true;
            T6 = true;
            T7 = true;
            T8 = true;
            T9 = true;
        end
        
        function [T0, T1, T2, T3, T4, T5, T6, T7, T8, T9] = getOutputDataTypeImpl(obj)
            T0 = 'int32';
            T1 = 'int32';
            T2 = 'int32';
            T3 = 'int32';
            T4 = 'int32';
            T5 = 'int32';
            T6 = 'int32';
            T7 = 'int32';
            T8 = 'int32';
            T9 = 'int32';
        end
        
        function [T0, T1, T2, T3, T4, T5, T6, T7, T8, T9] = isOutputComplexImpl(obj)
            T0 = false;
            T1 = false;
            T2 = false;
            T3 = false;
            T4 = false;
            T5 = false;
            T6 = false;
            T7 = false;
            T8 = false;
            T9 = false;
        end
    end
end