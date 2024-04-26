const toggleLivePlotButton: HTMLElement | null =
  document.getElementById("startstop");
const plot: ExtendedPlotlyHTMLElement = document.getElementById(
  "plot"
) as ExtendedPlotlyHTMLElement;

let refresh_interval: NodeJS.Timeout | null = null;
let last_pulled_ts: number = 0;
let plot_is_live: boolean = false;
let sensor_sampling_rate_hz: number | null;
let n_points_live_plot: number | null;
const plot_data: number[][] = [[], []];
const DEFAULT_PLOT_CONFIG: Partial<Plotly.Config> = { scrollZoom: true, staticPlot: true };

const DEFAULT_TRACE_CONFIG: Partial<Plotly.PlotData> = {
  mode: "lines",
  line: { color: "#80CAF6" },
  name: "flow",
};

const yAxis = {
  title: "Flow [sL/min]",
  showline: false,
};

const liveTemplate = {
  data: {},
  layout: {
    xaxis: {
      title: "time [s]",
      showgrid: false,
      zeroline: false,
    },

    yaxis: yAxis,
  },
};

const stoppedTemplate = {
  data: {},
  layout: {
    xaxis: {
      title: "timestamp [us]",
      showgrid: false,
      zeroline: false,
    },

    yaxis: yAxis,
  },
};

window.onload = displayPlot;

async function displayPlot(): Promise<void> {
  await initPlot();
}

async function initPlot(): Promise<void> {
  await Plotly.newPlot(
    plot,
    [
      {
        ...DEFAULT_TRACE_CONFIG,
        x: plot_data[0],
        y: plot_data[1],
      },
    ],
    { template: liveTemplate },
    DEFAULT_PLOT_CONFIG
  );

  toggleLivePlotButton?.addEventListener("click", toggleLivePLot);

  addPlotListeners();
}

function toggleLivePLot(): void {
  toggleLivePlotButton!.setAttribute("disabled", "true");

  if (plot_is_live) {
    toggleLivePlotButton!.innerText = "Connect";
    updateInfoBox("Disconnected", undefined, undefined);
    stopPlot();
    plot_is_live = false;
    toggleLivePlotButton?.removeAttribute("disabled");
  } else {
    toggleLivePlotButton!.innerText = "Connecting";
    startPlot();
  }
}

function addPlotListeners(): void {
  plot.on("plotly_relayout", async (event) => await onRelayout(event));
}

async function startPlot(): Promise<void> {
  //  Ensure plot data is cleared
  plot_data[0] = [];
  plot_data[1] = [];

  //  Request connection
  const hostname = await api.getHost();
  const clientInfos = await api.startDataAcquisition();

  if (clientInfos == null) {
    updateInfoBox("Connection Failed", hostname, undefined);
    toggleLivePlotButton!.innerText = "Connect";
    toggleLivePlotButton!.removeAttribute("disabled");
  } else {
    // Update sensor samplig rate
    sensor_sampling_rate_hz = clientInfos["sampling_frequency_milli_hz"] / 1000;
    n_points_live_plot =
      sensor_sampling_rate_hz *
      PLOT_LIVE_SUB_SAMPLING_RATIO *
      PLOT_LIVE_WINDOW_SEC;
    // Update UI
    updateInfoBox("Connected", hostname, clientInfos["sensor_name"]);
    plot_is_live = true;
    toggleLivePlotButton!.innerText = "Disconnect";
    toggleLivePlotButton!.removeAttribute("disabled");
    // Create timer to fetch data
    if (!refresh_interval) {
      refresh_interval = setInterval(
        updateLivePlotData,
        1000 / PLOT_LIVE_REFRESH_FREQ_HZ
      );
    }
  }
}

async function stopPlot(): Promise<void> {
  api.stopDataAcquisition();
  if (refresh_interval) {
    clearInterval(refresh_interval);
    refresh_interval = null;
  }
  await updatePlotConfig(plot, {
    ...DEFAULT_PLOT_CONFIG,
    staticPlot: false,
    displayModeBar: true,
  });
  // on stop, switch to real timestamps
  await Plotly.update(
    plot,
    { x: [plot_data[0]], y: [plot_data[1]] },
    { template: stoppedTemplate }
  );
}

async function updateLivePlotData(): Promise<void> {
  const new_data: number[][] = await api.getDataSinceTimestamp(
    last_pulled_ts,
    PLOT_LIVE_SUB_SAMPLING_RATIO
  );
  plot_data[0] = plot_data[0].concat(new_data[0]);
  plot_data[1] = plot_data[1].concat(new_data[1]);

  last_pulled_ts = new_data[0].at(-1) || last_pulled_ts;

  const n_points = plot_data[0].length;

  if (n_points > n_points_live_plot!) {
    plot_data[0].splice(0, n_points - n_points_live_plot!);
    plot_data[1].splice(0, n_points - n_points_live_plot!);
  }

  const x_axis_live = Array.from(
    { length: n_points_live_plot! },
    (_, i) => (-(n_points - i) * PLOT_LIVE_WINDOW_SEC) / n_points_live_plot!
  );

  const data_update = {
    x: [x_axis_live],
    y: [plot_data[1]],
  };

  if (plot._context.staticPlot) {
    await Plotly.update(plot, data_update, plot.layout);
  } else {
    await updatePlotConfig(plot, DEFAULT_PLOT_CONFIG);
  }
}

async function updatePlotConfig(
  plot: ExtendedPlotlyHTMLElement,
  config?: Partial<Plotly.Config>
): Promise<void> {
  await Plotly.newPlot(plot, plot.data, { template: liveTemplate }, config);
  addPlotListeners();
}

async function onRelayout(event: Plotly.PlotRelayoutEvent): Promise<void> {
  // Events that can change the axes
  const relevantEvents = {
    xAuto: event["xaxis.autorange"],
    yAuto: event["yaxis.autorange"],
    x0: event["xaxis.range[0]"],
    x1: event["xaxis.range[1]"],
    y0: event["yaxis.range[0]"],
    y1: event["yaxis.range[1]"],
  };

  const isAxisAffected = Object.values(relevantEvents).some(
    (prop) => prop !== undefined
  );

  if (!isAxisAffected) {
    return;
  }

  // Get axes directly from plot, not from event as they are not always defined in event
  const axes = {
    x: [plot.layout.xaxis.range?.at(0), plot.layout.xaxis.range?.at(1)],
    y: [plot.layout.yaxis.range?.at(0), plot.layout.yaxis.range?.at(1)],
  };

  await updateRangePlotData(axes.x[0], axes.x[1]);
}

async function updateRangePlotData(
  tsStart: number,
  tsStop: number
): Promise<void> {
  // replace existing trace with range data without subsampling
  const range_data: number[][] = await api.getDataInRange(tsStart, tsStop, 1);

  await Plotly.deleteTraces(plot, 0);
  await Plotly.addTraces(plot, [
    {
      ...DEFAULT_TRACE_CONFIG,
      x: range_data[0],
      y: range_data[1],
    },
  ]);
}

function updateInfoBox(
  connectionState?: string,
  hostname?: string,
  sensorType?: string
) {
  if (connectionState) {
    document.getElementById("connStatus")!.innerText = connectionState;
  }
  if (hostname) {
    document.getElementById("hostname")!.innerText = hostname;
  }
  if (sensorType) {
    document.getElementById("sensorType")!.innerText = sensorType;
  }
}
