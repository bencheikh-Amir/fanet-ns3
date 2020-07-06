function create_bar_figure(ymatrix1, nodes, r, mob_name, vel)
%CREATEFIGURE(YMATRIX1)
%  YMATRIX1:  bar matrix data

%  Auto-generated by MATLAB on 28-Jun-2020 21:29:55

% Create figure
figure1 = figure;

% Create axes
axes1 = axes('Parent',figure1,...
    'Position',[0.13 0.215586362245769 0.775 0.718311364909476]);
hold(axes1,'on');

% Create multiple lines using matrix input to bar
bar1 = bar(ymatrix1,'Parent',axes1);
for i = 1:length(nodes)
    set(bar1(i),'DisplayName', sprintf('n = %d', nodes(i)));
end

%set(bar1(4),'DisplayName','n = 4');
%set(bar1(3),'DisplayName','n = 8');
%set(bar1(2),'DisplayName','n = 12');
%set(bar1(1),'DisplayName','n = 16');

% Create title
title(sprintf('Реальная связность сети в модели мобильности %s, v=%d', mob_name, vel));

% Create ylabel
ylabel('Коэффициент связности');

box(axes1,'on');
grid(axes1,'on');
% Set the remaining axes properties
set(axes1,'FontSize',18,'XTick',[1 2 3],'XTickLabel',r);
% Create legend
legend1 = legend(axes1,'show');
set(legend1,...
    'Position',[0.2836897801644 0.0968169761273209 0.46851144196423 0.0536441580287994],...
    'Orientation','horizontal',...
    'FontSize',18);

name=sprintf('%s_%f.png', mob_name, vel);
set(gcf, 'Position', [0, 0, 1000, 600]);
saveas(gcf, name);
export_fig MOB_V;