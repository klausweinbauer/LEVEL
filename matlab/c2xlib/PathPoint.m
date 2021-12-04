classdef PathPoint < matlab.System
    
    properties (Nontunable)          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [PathPoint] = stepImpl(obj, DeltaLatitude, DeltaLongitude, DeltaAltitude, PathDeltaTime) 
            PathPoint = int32(transpose([DeltaLatitude DeltaLongitude DeltaAltitude PathDeltaTime]));
        end
        
        function releaseImpl(~)            
        end
    end

    methods (Access = protected)
        function [PathPoint] = getOutputSizeImpl(obj)
            PathPoint = [4 1];
        end 
        
        function [PathPoint] = isOutputFixedSizeImpl(obj)
            PathPoint = true;
        end
        
        function [PathPoint] = getOutputDataTypeImpl(obj)
            PathPoint = 'int32';
        end
        
        function [PathPoint] = isOutputComplexImpl(obj)
            PathPoint = false;
        end
    end
end