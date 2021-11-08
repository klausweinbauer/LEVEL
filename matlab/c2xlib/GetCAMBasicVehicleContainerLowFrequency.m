classdef GetCAMBasicVehicleContainerLowFrequency < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        ExteriorLightsSize = 0;  
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [VehicleRole, ExteriorLights, ActualExteriorLightsSize] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.ceval('getCAMBasicVehicleContainerLowFrequency', StationID, coder.wref(VehicleRole), coder.wref(ExteriorLights), obj.ExteriorLightsSize, coder.wref(ActualExteriorLightsSize));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMBasicVehicleContainerLowFrequency';
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