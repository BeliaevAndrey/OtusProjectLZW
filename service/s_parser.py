#! /bin/env python3

import sys
import os
import pprint
import csv

SEP = "=" * 30


def parse_section(section: str, results: dict):
    if "Compressing" in section:
        parse_section_cmp(section, results)
    if "Decompressing" in section:
        parse_section_dcmp(section, results)


def parse_section_cmp(section: str, results: dict):
    local_dict = {}

    act = ""
    file_name = ""

    for line in section.split("\n"):
        if "file_in" in line:
            file_name = line.rsplit('/', 1)[1]
            local_dict["file_in"] = line.rsplit('/', 1)[1]
            local_dict["type"] = file_name.rsplit('.', 1)[1]
            file_name = file_name.rsplit('.', 1)[0]
        elif "file_out" in line:
            local_dict["file_out"] = line.rsplit('/', 1)[1]
        elif "File size" in line:
            local_dict["File size"] = int(line.rsplit(': ', 1)[1].replace('bytes', ''))
        elif "Compressed size" in line:
            local_dict["Compressed size"] = int(line.rsplit(': ', 1)[1].replace('bytes', ''))
        elif "Compression ratio" in line:
            local_dict["Compression ratio"] = float(line.rsplit(': ', 1)[1].replace('%', ''))
            act = "compression"
        elif "time taken" in line:
            local_dict["time compression"] = int(line.rsplit(': ', 1)[1].replace('seconds', ''))
       
    
    if file_name in results:
        results[file_name][act] = local_dict
    else:
        results[file_name] = {}
        results[file_name][act]=local_dict


def parse_section_dcmp(section: str, results: dict):
    local_dict = {}

    act = ""
    file_name = ""

    for line in section.split("\n"):
        if "file_in" in line:
            file_name = line.rsplit('/', 1)[1]
            if ".lzwbin" in file_name:
                file_name = file_name.rsplit('.', 1)[0]
            local_dict["file_in"] = line.rsplit('/', 1)[1]
            local_dict["type"] = file_name.rsplit('.', 1)[1]
            file_name = file_name.rsplit('.', 1)[0]
        elif "file_out" in line:
            local_dict["file_out"] = line.rsplit('/', 1)[1]
        elif "time taken" in line:
            local_dict["time decompression"] = int(line.rsplit(': ', 1)[1].replace('seconds', ''))
        elif "Compressed data size" in line:
            local_dict["Compressed data size"] = int(line.rsplit(': ', 1)[1].replace('bytes', ''))
        elif "Decompressed data size" in line:
            local_dict["Decompressed data size"] = int(line.rsplit(': ', 1)[1].replace('bytes', ''))
            act = "decompression"
        
    
    if file_name in results:
        results[file_name][act] = local_dict
    else:
        results[file_name] = {}
        results[file_name][act]=local_dict



def parse_dict(results: dict):
    keys = ['file_in', 'type', 'File size', 
            'Compressed size', 'Compression ratio', 'Compressed data size', 'Decompressed data size', 
            'time compression', 'time decompression', ]
    
    table = []
    for name in results.keys():
        section = []
        for key in keys[:-2]:
            section.append(results[name]["compression"].get(key)
            or               
            results[name]["decompression"].get(key))
        section.append(results[name]["compression"]['time compression'])
        section.append(results[name]["decompression"]['time decompression'])
        section[0] = name
        table.append(section)
    table.sort(key=lambda x: x[2])
    table.sort(key=lambda x: x[1])
    table = [keys] + table
    # pprint.pp(table)
    return table



def write_csv(table: list, file_path: str, file: str):
    table[0][0] = "File name"
    table[0] = [head.replace('_', ' ').capitalize() for head in table[0]]
    csv_file = os.path.join(file_path, file)
    with open(csv_file, 'w', encoding='utf-8') as file_out:
        csv.writer(file_out, dialect='excel', delimiter=',').writerows(table)
    return table


def open_file(path: str) -> list:
    data = []
    with open(path, 'r', encoding='utf-8') as file_in:
        data = file_in.read().split(SEP)
    return data



def parse_args(args: list[str]):
    path, log_file = args[1].rsplit('/', 1)
    path = os.path.abspath(path)
    log_data = open_file(os.path.join(path, log_file))

    results = dict()
    for section in log_data:
        parse_section(section, results)

    csv_path = path
    csv_file = "results.csv"
    return write_csv(parse_dict(results), csv_path, csv_file)
    # return results


if __name__ == '__main__':

    if len(sys.argv) > 1:
        pprint.pp(parse_args(sys.argv))
    else:
        usage = """
        Usage:
        parse_logs.py <filename>
        """
        print(usage)

