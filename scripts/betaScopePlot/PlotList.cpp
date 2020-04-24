#include "plotList.h"

std::vector<PlotList> create_job() {
  BookPlot my_booked_plot;

  my_booked_plot.book_plot(2, "pmax2", "Pmax_Ch2", "Pmax ch-2", "pmax2[0]",
                           "Pmax Ch2 [mV]", "LanGaus", 100, 0, 400);
  my_booked_plot.book_plot(3, "pmax3", "Pmax_Ch3", "Pmax ch-3", "pmax3[0]",
                           "Pmax Ch3 [mV]", "LanGaus", 60, 0, 400);

  my_booked_plot.book_plot(2, "rms2", "RMS_Ch2", "RMS ch-2", "rms2[0]",
                           "Noise Ch2 [mV]", "gaus", 100, 0, 10);
  my_booked_plot.book_plot(3, "rms3", "RMS_Ch3", "RMS ch-3", "rms3[0]",
                           "Noise Ch3 [mV]", "gaus", 100, 0, 10);

  my_booked_plot.book_plot(2, "rise2", "Rise_Time_Ch2", "Rise Time ch-2",
                           "riseTime2[0]", "Rise Time Ch2 [ps]", "gaus", 100, 0,
                           2000);
  my_booked_plot.book_plot(3, "rise3", "Rise_Time_Ch3", "Rise Time ch-3",
                           "riseTime3[0]", "Rise Time Ch3 [ps]", "gaus", 100, 0,
                           2000);

  my_booked_plot.book_plot(2, "fall2", "Fall_Time_Ch2", "Fall Time ch-2",
                           "cfd_fall2[10]-cfd_fall2[90]", "Fall Time Ch2 [ps]",
                           "gaus", 100, 0, 5000);
  my_booked_plot.book_plot(3, "fall3", "Fall_Time_Ch3", "Fall Time ch-3",
                           "cfd_fall3[10]-cfd_fall3[90]", "Fall Time Ch3 [ps]",
                           "gaus", 100, 0, 5000);

  my_booked_plot.book_plot(2, "dvdt2", "dvdt_20per_Ch2", "dvdt 20% ch-2",
                           "dvdt2[20]", "dvdt Ch2 [V/ps]", "gaus", 100, 0,
                           1000);
  my_booked_plot.book_plot(3, "dvdt3", "dvdt_20per_Ch3", "dvdt 20% ch-3",
                           "dvdt3[20]", "dvdt Ch3 [V/ps]", "gaus", 100, 0,
                           1000);

  my_booked_plot.book_plot(2, "jitter2", "jitter_20per_Ch2", "jitter 20% ch-2",
                           "rms2[0]/dvdt2[20]", "Jitter Ch2 [ps]", "landau",
                           100, 0, 200);
  my_booked_plot.book_plot(3, "jitter3", "jitter_20per_Ch3", "jitter 20% ch-3",
                           "rms3[0]/dvdt3[20]", "Jitter Ch3 [ps]", "landau",
                           100, 0, 200);

  my_booked_plot.book_plot(2, "fwhm2", "fwhm_Ch2", "FWHM ch-2",
                           "cfd_fall2[50]-cfd2[50]", "FWHM Ch2 [ps]", "gaus",
                           100, 0, 5000);
  my_booked_plot.book_plot(3, "fwhm3", "fwhm_Ch3", "FWHM ch-3",
                           "cfd_fall3[50]-cfd3[50]", "FWHM Ch3 [ps]", "gaus",
                           100, 0, 5000);

  my_booked_plot.book_plot(2, "area_zero2", "area_zeroCross_Ch2",
                           "Area with zero-cross ch-2",
                           "pulseArea_withZeroCross2[0]/1.0E-15", "Area 0X Ch2",
                           "LanGausArea", 100, 0, 20000);
  my_booked_plot.book_plot(3, "area_zero3", "area_zeroCross_Ch3",
                           "Area with zero-cross ch-3",
                           "pulseArea_withZeroCross3[0]/1.0E-15", "Area 0X Ch3",
                           "LanGausArea", 100, 0, 20000);

  my_booked_plot.book_plot(2, "area_under2", "area_underShoot_Ch2",
                           "Area with undershoot corr ch-2",
                           "pulseArea_withUndershoot2[0]/1.0E-15",
                           "Area Under Ch2", "LanGausArea", 100, 0, 20000);
  my_booked_plot.book_plot(3, "area_under3", "area_underShoot_Ch3",
                           "Area with undershoot corr ch-3",
                           "pulseArea_withUndershoot3[0]/1.0E-15",
                           "Area Under Ch3", "LanGausArea", 100, 0, 20000);

  return my_booked_plot.get_booked_plot();
}
