#!/usr/bin/env python3
# (c) 2021 Thijs Takken
__version__ = '1.0 2021-12-23'

def control(state, addr):
    # Send the url request to the LED control function
    # Hints: use the addr (IP) and state variables

    try:
        # Trainee work {{
        urllib.request.urlopen('http://' + str(addr) + "/set_power?state=" + state)
        contents = print('http://' + str(addr) + "/set_power?state=" + state)
        # Trainee work }}

        return contents

    # Ignore unavailable hosts
    except IOError:
        pass

gVbs = False
if __name__ == '__main__':

    import urllib.request
    import ipaddress
    from multiprocessing.pool import ThreadPool as Pool

    # Network address with the subnet of the corresponding network
    # Example: '192.168.1.0/24'
    hosts = '192.168.40.0/24'

    # Numbers of hosts counter
    # Set tuple constructor
    hostsnr = tuple()
    # Count the number of hosts
    for interation in enumerate(ipaddress.IPv4Network(hosts)):
        hostsnr = interation

    # Save first item in the list
    hostsnr = hostsnr[0]

    # State: 1 = On and 0 = Off
    # Set initial state to 0
    state = 0

    # Loop for LED on and off
    while True:
        # If LED is on set it to off and vice versa
        mesg = str()
        if state == 1:
            state = 0
            mesg = 'Disabled'
        elif state == 0:
            state = 1
            mesg = 'Enabled'

        # Set asynchronous pool to number of hosts + 1
        pool_size = (hostsnr + 1)  # Parallelness
        pool = Pool(pool_size)

        # Every cycle takes around 20 seconds, this is due to urllib which waits 20 seconds
        # For every available host in the IP given range call the control function
        for addr in ipaddress.IPv4Network(hosts):
            # Send LED command to all available hosts on the network asynchronously (multi thread operation)
            contents = pool.apply_async(control, (str(state), addr,))

            # Debug purposes, turn the comment off to make use of it
            #print(contents)
            #print(addr)

        print('')
        print('----------------------------------------')
        print('Command to ' + hosts + ' network send')

        print('to set the LEDs to ' + mesg)

        print('')

        print('LED update successfull on:')


        pool.close()
        pool.join()