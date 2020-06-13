function create_connectivity_figure(X1, X2, YMatrix1, YMatrix2, YMatrix3, text, ttl)
%CREATEFIGURE(X1, YMATRIX1, YMATRIX2, YMATRIX3)
%  X1:  vector of x data
%  YMATRIX1:  matrix of y data
%  YMATRIX2:  matrix of y data
%  YMATRIX3:  matrix of y data

%  Auto-generated by MATLAB on 09-Jun-2020 16:44:50

% Create figure
figure1 = figure;

% Create subplot
subplot1 = subplot(3,1,1,'Parent',figure1);
hold(subplot1,'on');

% Create multiple lines using matrix input to plot
plot1 = plot(X1,YMatrix1,'Parent',subplot1,'MarkerSize',8,'Marker','o',...
    'LineWidth',1,...
    'LineStyle',':');
ylim([0, 1.0]);
for i = 1:length(X2)
    set(plot1(i),'DisplayName', strcat(text, num2str(X2(i))));
end

% Create title
title(sprintf('Зависимость коэффициента связности от скорости узлов (%s)', ttl));

grid(subplot1,'on');
% Create subplot
subplot2 = subplot(3,1,2,'Parent',figure1);
hold(subplot2,'on');

% Create multiple lines using matrix input to plot
plot2 = plot(X1,YMatrix2,'Parent',subplot2,'MarkerSize',8,'Marker','o',...
    'LineWidth',1,...
    'LineStyle',':');
ylim([0, 1.0]);
for i = 1:length(X2)
    set(plot2(i),'DisplayName', strcat(text, num2str(X2(i))));
end

% Create ylabel
ylabel('Коэффициент связности');

grid(subplot2,'on');
% Create subplot
subplot3 = subplot(3,1,3,'Parent',figure1);
hold(subplot3,'on');

% Create multiple lines using matrix input to plot
plot3 = plot(X1,YMatrix3,'Parent',subplot3,'MarkerSize',8,'Marker','o',...
    'LineWidth',1,...
    'LineStyle',':');
ylim([0, 1.0]);
for i = 1:length(X2)
    set(plot3(i),'DisplayName', strcat(text, num2str(X2(i))));
end

% Create xlabel
xlabel('Скорость, м/с');

grid(subplot3,'on');
% Create legend
legend1 = legend(subplot3,'show');
set(legend1,...
    'Position',[0.33025641025641 0.0107015457788347 0.368205128205128 0.0487514863258025],...
    'Orientation','horizontal');

