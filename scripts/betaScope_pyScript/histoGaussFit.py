import ROOT


def histoGaussFit(tfile_name, var_express, cuts, return_histo=False):
    tfile = ROOT.TFile.Open(tfile_name, "r")
    ttree_wfm = tfile.wfm

    # create default histogram for pre-processing.
    preHisto = ROOT.TH1D("prepHisto", "prepHisto", 100, 1, 1)
    ttree_wfm.Project("prepHisto", var_express, cuts)
    sample_std = preHisto.GetStdDev(1)
    sample_mean = preHisto.GetMean(1)
    num_events = preHisto.GetEntries()
    IQR = 2 * 0.67845 * sample_std  # might not be the correct one

    # cfd time different histogram for timing resolution extraction
    bin_width = 2 * IQR / pow(num_events, 1.0 / 3.0)
    min_range = sample_mean - 5.0 * sample_std
    max_range = sample_mean + 5.0 * sample_std
    num_bins = int((max_range - min_range) / bin_width)
    tdiff_histo = ROOT.TH1D("postHisto", "postHisto", num_bins, min_range, max_range)
    ttree_wfm.Project("postHisto", var_express, cuts)
    gaussian = ROOT.TF1("gaussian", "gaus")
    tdiff_histo.Fit(gaussian)
    mean = gaussian.GetParameter(1)
    mean_err = gaussian.GetParError(1)
    sigma = gaussian.GetParameter(2)
    sigma_err = gaussian.GetParError(2)

    # returning histogram
    if return_histo:
        tdiff_histo.SetDirectory(0)
        return {
            "histo": tdiff_histo,
            "sigma": sigma,
            "sigma_err": sigma_err,
            "mean": mean,
            "mean_err": mean_err,
        }
    else:
        return {
            "sigma": sigma,
            "sigma_err": sigma_err,
            "mean": mean,
            "mean_err": mean_err,
        }
