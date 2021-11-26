classdef EventPosition_concat < matlab.System
    
    properties (Nontunable)
    end      
    
    methods (Access = protected)
        function setupImpl(~)
        end
        function [EventPosition_Vector] = stepImpl(~, Latitude, Longitude, PosConfEllSemiMajorConf, ...
                PosConfEllSemiMinorConf, PosConfEllSemiMajorOri, ...
                AltitudeValue, AltitudeConf)
            EventPosition_Vector = transpose([
                int32(Latitude), int32(Longitude), int32(PosConfEllSemiMajorConf), ...
                int32(PosConfEllSemiMinorConf), int32(PosConfEllSemiMajorOri), int32(AltitudeValue), ... 
                int32(AltitudeConf)]);
        end
        function releaseImpl(~)            
        end
    end
    
    methods (Access = protected)
       function [sz_1] = getOutputSizeImpl(~)
          sz_1 = [7, 1];
       end 
       
       function [c1] = isOutputFixedSizeImpl(~)
          c1 = true;
       end
       
       function [dt1] = getOutputDataTypeImpl(~)
         dt1 = "int32";
       end
       
       function [co1] = isOutputComplexImpl(~)
           co1 = false;
       end
    end
end