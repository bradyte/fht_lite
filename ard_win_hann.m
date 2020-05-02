MAX_RES = 2^15; % signed 16 bit max resolution
NFHT = 256;

wx = zeros(NFHT,1);
w_hann = round(MAX_RES*hann(NFHT));
% plot(w_hann,'r.'); axis([0 NFHT 0 MAX_RES]); grid on;

x  = [ ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150, ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150, ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150, ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150, ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150, ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150, ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150, ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150, ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150, ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150, ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150, ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150, ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150, ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150, ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150, ...
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150]';

for i=1:length(x)
    x(i) = x(i) - 512;
    x(i) = bitshift(x(i),6,'int16');
end

wx = floor(x.*w_hann/2^15);

plot(wx,'k.:');