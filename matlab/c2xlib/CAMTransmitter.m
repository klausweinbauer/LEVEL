classdef CAMTransmitter < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        Port = 1997;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(obj)
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.ceval('startCAMTransmitter', obj.Port);
            end 
        end
        
        function [] = stepImpl(obj, IDsToTransmit, f_send) 
            Size = length(IDsToTransmit);
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.ceval('setCAMTransmissionFrequency', f_send);
                coder.ceval('setCAMIDsForTransmission', coder.wref(IDsToTransmit), Size);
            end            
        end
        
        function releaseImpl(~)   
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.ceval('stopCAMTransmitter');
            end          
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'CAMTransmitter';
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
            libName =  strcat('c2xcam', linkLibExt);
            % Other linking parameters
            libPath = './';
            libPriority = '';
            libPreCompiled = true;
            libLinkOnly = true;

            % Linking command
            buildInfo.addLinkObjects(libName,libPath,libPriority,libPreCompiled,libLinkOnly);
        end
    end
end