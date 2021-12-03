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
    end


end