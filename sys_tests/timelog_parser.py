from operator import sub

def parse_timings(filename):
    '''Load timings log and parse it'''
    headers = {}
    entries = {}
    with open(filename, 'r') as f:
        for line in f.readlines():
            if line.startswith('Header'):
                record_name, fields = line.split(':')[1:]
                record_name = record_name.strip()
                entries[record_name] = {}
                header = []
                for i, field in enumerate(fields.strip().split(',')):
                    field_name, field_type = field.split('=')
                    header.append((field_name, field_type))
                    entries[record_name][field_name] = []
                headers[record_name] = header
            elif line.startswith('Data'):
                record_name, fields = line.split(':')[1:]
                record_name = record_name.strip()
                assert record_name in headers
                fields = fields.split(',')
                header = headers[record_name]
                for i, field in enumerate(fields):
                    field_value = field.split('=')[1]
                    entries[record_name][header[i][0]].append(field_value.strip())
    # convert types
    for record_name, header in headers.iteritems():
        for field_name, field_type in header:
            if field_type in ['uint64', 'timestamp']:
                entries[record_name][field_name] = [int(val) for val in entries[record_name][field_name]]

    timings = entries['TimeLogEntry']
    # generate derived fields
    timings['Parsing'] = map(sub, timings['finishParsingTS'], timings['startTS'])
    timings['Processing'] = map(sub, timings['finishProcessingTS'], timings['finishParsingTS'])
    timings['Send'] = map(sub, timings['finishSendTS'], timings['finishProcessingTS'])
    timings['Total'] = map(sub, timings['finishSendTS'], timings['startTS'])

    return entries
