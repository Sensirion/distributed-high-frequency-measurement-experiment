interface ExtendedPlotlyHTMLElement extends Plotly.PlotlyHTMLElement {
    _context: PlotlyHTMLElementContext
}

interface PlotlyHTMLElementContext {
    // only way to get config info about plot that is already drawn
    staticPlot: boolean
}