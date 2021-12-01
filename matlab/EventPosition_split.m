classdef EventPosition_split < matlab.System
    
    properties (Nontunable)
    end      
    
    methods (Access = protected)
        function setupImpl(~)
        end
        function [Latitude, Longitude, PosConfEllSemiMajorConf, ...
                PosConfEllSemiMinorConf, PosConfEllSemiMajorOri, ...
                AltitudeValue, AltitudeConf] = stepImpl(~, EventPosition_Vector)
            
            vec = double(EventPosition_Vector);
            Latitude = 0;
            Longitude = 0;
            PosConfEllSemiMajorConf = 0;
            PosConfEllSemiMinorConf = 0;
            PosConfEllSemiMajorOri = 0;
            AltitudeValue = 0;
            AltitudeConf = 0;
            
            if length(vec) == 7
                Latitude = vec(1);
                Longitude = vec(2);
                PosConfEllSemiMajorConf = vec(3);
                PosConfEllSemiMinorConf = vec(4);
                PosConfEllSemiMajorOri = vec(5);
                AltitudeValue = vec(6);
                AltitudeConf = vec(7);
            end
        end
        function releaseImpl(~)            
        end
    end
end