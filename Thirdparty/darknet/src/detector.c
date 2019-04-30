#include "darknet.h"
#include "network.h"
#include "region_layer.h"
#include "cost_layer.h"
#include "utils.h"
#include "parser.h"
#include "box.h"
#include "demo.h"
#include "option_list.h"

#ifndef __COMPAR_FN_T
#define __COMPAR_FN_T
typedef int (*__compar_fn_t)(const void*, const void*);
#ifdef __USE_GNU
typedef __compar_fn_t comparison_fn_t;
#endif
#endif

#include "http_stream.h"
#include "detector.h"

// Additional properties.
static list *options;
static network net;
static char *name_list;
static char **names;

void setup_detector() {
    char *cfgfile = "cfg/palm.cfg";
    options = read_data_cfg(cfgfile);
    net = parse_network_cfg_custom("cfg/palm.cfg", 1, 1); // set batch=1
    load_weights(&net, "");
    name_list = option_find_str(options, "names", "cfg/names.list");
    int names_size = 0;
    names = get_labels_custom(name_list, &names_size); //get_labels(name_list);
    fuse_conv_batchnorm(net);
    calculate_binary_weights(net);
    if (net.layers[net.n - 1].classes != names_size) {
        printf(" Error: in the file %s number of names %d that isn't equal to classes=%d in the file %s \n",
               name_list, names_size, net.layers[net.n - 1].classes, cfgfile);
        if (net.layers[net.n - 1].classes > names_size) getchar();
    }
}

detection* detect(char *filename, float thresh, float hier_thresh)
{

    image **alphabet = load_alphabet();
//    srand(2222222);
    char buff[256];
    char *input = buff;
    int j;
    float nms = .45;    // 0.4F
    detection *dets;
    while (1) {
        if (filename) {
            strncpy(input, filename, 256);
            if (strlen(input) > 0)
                if (input[strlen(input) - 1] == 0x0d) input[strlen(input) - 1] = 0;
        }
        else {
            printf("Enter Image Path: ");
            fflush(stdout);
            input = fgets(input, 256, stdin);
            if (!input) break;
            strtok(input, "\n");
        }
        image im = load_image(input, 0, 0, net.c);
        image sized = resize_image(im, net.w, net.h);
        int letterbox = 0;
        layer l = net.layers[net.n - 1];

        float *X = sized.data;

        double time = get_time_point();
        network_predict(net, X);
        printf("%s: Predicted in %lf milli-seconds.\n", input, ((double)get_time_point() - time) / 1000);

        int nboxes = 0;
        dets = get_network_boxes(&net, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes, letterbox);
        if (nms) do_nms_sort(dets, nboxes, l.classes, nms);
        free_image(im);
        free_image(sized);

        if (filename) break;
    }

    int i;
    const int nsize = 8;
    for (j = 0; j < nsize; ++j) {
        for (i = 32; i < 127; ++i) {
            free_image(alphabet[j][i]);
        }
        free(alphabet[j]);
    }
    free(alphabet);

    return dets;
}