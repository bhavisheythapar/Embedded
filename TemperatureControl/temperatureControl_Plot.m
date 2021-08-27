clear all;
close all;
clc;

s = serialport("COM3",9600);
target=0;
y1=yline(target,'-','Target','LineWidth',1.5);
i=1;
pause(15);

while (1)
    
    serial_data=readline(s);
    serial_data = split(serial_data,",");
    current_temp=str2double(serial_data(1));
    desired_temp=str2double(serial_data(2));
    
    hold on;
    if (target ~= desired_temp)
        delete(y1);
        target=desired_temp;
        y1=yline(target,'-','Target','LineWidth',1.5,'color','Black');
    end
    
    plot_data(i)=current_temp;
    plot (plot_data,'LineWidth',1.5,'color','Red')
    ylim([20 50])
    pause(0.05);
    i=i+1;
    
end
