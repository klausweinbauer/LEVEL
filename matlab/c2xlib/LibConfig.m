classdef LibConfig

    methods (Static)

        function path = getBasePath(~)
            path = 'C:\Program Files\c2xlib';
        end

        function path = getDLLPath(~)
            path = 'C:\Program Files\c2xlib\lib';
        end

        function path = getIncludePath(~)
            path = 'C:\Program Files\c2xlib\include';
        end

        function path = getCAMHeader(~)
            path = 'C:\Program Files\c2xlib\include\c2xcam.h';
        end

        function updateBuildInfoCAM(buildInfo, buildContext)
            [~, linkLibExt, execLibExt, ~] = buildContext.getStdLibInfo();

            % Parametrize library extension
            libName =  strcat('c2xcam', linkLibExt);
            % Other linking parameters
            
            libPriority = '';
            libPreCompiled = true;
            libLinkOnly = true;

            % Linking command
            libDLLPath = LibConfig.getDLLPath();
            libIncludePath = LibConfig.getIncludePath();
            buildInfo.addLinkObjects(libName,libDLLPath,libPriority,libPreCompiled,libLinkOnly);
            buildInfo.addIncludePaths(libIncludePath);
            buildInfo.addIncludeFiles('c2xcommon.h');
        end

        function updateBuildInfoDENM(buildInfo, buildContext)
            [~, linkLibExt, execLibExt, ~] = buildContext.getStdLibInfo();

            % Parametrize library extension
            libName =  strcat('c2xdenm', linkLibExt);
            % Other linking parameters
            
            libPriority = '';
            libPreCompiled = true;
            libLinkOnly = true;

            % Linking command
            libDLLPath = LibConfig.getDLLPath();
            libIncludePath = LibConfig.getIncludePath();
            buildInfo.addLinkObjects(libName,libDLLPath,libPriority,libPreCompiled,libLinkOnly);
            buildInfo.addIncludePaths(libIncludePath);
            buildInfo.addIncludeFiles('c2xcommon.h');
        end
    end


end