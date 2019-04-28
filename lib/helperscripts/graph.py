import sys
import json

import networkx
import matplotlib.pyplot as plt
import networkx as nx
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib as mpl
1
2
3
4
5
import networkx as nx
import random
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
def main(args):
    marksize = mpl.rcParams['lines.markersize'] ** 2
    rf = open(args[0], 'r')
    parsed = json.loads(rf.read())
    with plt.style.context(('ggplot')):
        struct = {}
        struct['nodes'] = {}
        struct['edges'] = []
        for inp in parsed['inputs']:
            struct['nodes'][inp['uid']] = {}
            struct['nodes'][inp['uid']]['location'] = inp['position']
            struct['nodes'][inp['uid']]['color'] = 'r'
            struct['nodes'][inp['uid']]['size'] = marksize * 4
            struct['nodes'][inp['uid']]['alpha'] = 1
        for inp in parsed['mid_neurons']:
            struct['nodes'][inp['uid']] = {}
            struct['nodes'][inp['uid']]['location'] = inp['position']
            struct['nodes'][inp['uid']]['color'] = 'g'
            struct['nodes'][inp['uid']]['size'] = marksize
            struct['nodes'][inp['uid']]['alpha'] = 0.05
            for conn in inp['input_connections']:
                new_edge = {}
                new_edge['source'] = conn['uid']
                new_edge['target'] = inp['uid']
                struct['edges'].append(new_edge)
        for inp in parsed['outputs']:
            struct['nodes'][inp['uid']] = {}
            struct['nodes'][inp['uid']]['location'] = inp['position']
            struct['nodes'][inp['uid']]['color'] = 'b'
            struct['nodes'][inp['uid']]['size'] = marksize * 4
            struct['nodes'][inp['uid']]['alpha'] = 1
            for conn in inp['input_connections']:
                new_edge = {}
                new_edge['source'] = conn['uid']
                new_edge['target'] = inp['uid']
                new_edge['alpha'] = conn['alpha']
                new_edge['weight'] = conn['weight']
                struct['edges'].append(new_edge)

        uids = struct['nodes'].keys()
        pos = [node['location'] for node in struct['nodes'].values()]
        posdict = {}
        for node in struct['nodes'].keys():
            posdict[node] = struct['nodes'][node]['location']
        edges = [(edge['source'], edge['target']) for edge in struct['edges']]


        fig = plt.figure(figsize=(10, 7))
        ax = Axes3D(fig)
        for key, value in struct['nodes'].items():
            xi = value['location'][0]
            yi = value['location'][1]
            zi = value['location'][2]

            ax.scatter(xi, yi, zi, 
                    color = value['color'],
                    alpha = value['alpha'],
                    s = value['size'])
        for edge in edges:
            spos = struct['nodes'][edge[0]]['location']
            dpos = struct['nodes'][edge[1]]['location']
            x = (spos[0], dpos[0])
            y = (spos[1], dpos[1])
            z = (spos[2], dpos[2])

            ax.plot(x, y, z, c='black', alpha=0.01)

        ax.view_init(30, 30)
        ax.set_axis_off()
        plt.show()


if(__name__ == '__main__'):
    main(sys.argv[1:])
