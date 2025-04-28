### centerpoint_pointpillar

The preprocessing step of the `centerpoint_pointpillar` model can be deployed on
either the CPU or the DSP, depending on the configuration specified in the file
`../../config/preprocess/centerpoint_pointpillar_5dim.json`. If the `run_on_dsp`
parameter in the config file is set to true, the preprocessing will be executed
on the DSP. Otherwise, it will run on the CPU. To observe the performance
differences, you can run the latency.sh script. In general, when running on the
DSP, the preprocessing latency is typically lower compared to running it on the
CPU.

Example of centerpoint_pointpillar preprocess configuration file.

```json
{
  "dim": 5,
  "max_num_point": 40000,
  "max_num_point_pillar": 20,
  "align_padding_point": 40064,
  "back": -51.2,
  "front": 51.2,
  "right": -51.2,
  "left": 51.2,
  "bottom": -5,
  "top": 3,
  "r_lower": 0.0,
  "r_upper": 255.0,
  "x_scale": 0.2,
  "y_scale": 0.2,
  "x_bev_scale": 0.1,
  "y_bev_scale": 0.1,
  "run_on_dsp": false
}
```

To make running preprocess on dsp, you must deploy dsp to development board
correctly. A deployment script which is located in `dsp_image` is provided for
deploying dsp easily.

```bash
# deploy dsp image to development board
cd dsp_image
bash deploy_dsp.sh
```

DSP supports running in two modes: direct mode and relay mode. By default, the
running mode is set to direct. However, if a relay server is already running,
you must switch the running mode to relay or terminate the `ucp_service`.

```bash
# set dsp running mode to direct
export HB_UCP_ENABLE_RELAY_MODE=false

# set dsp running mode to relay
export HB_UCP_ENABLE_RELAY_MODE=true
```
