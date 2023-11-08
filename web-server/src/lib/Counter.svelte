<script lang="ts">
  let count: number = 0
  const increment = () => {
    count += 1
  }
</script>

<button on:click={increment}>
  count12345 is {count}
</button>


<!-- TODO: Create a web-interface to control the arduino, thinking maybe creating a custom interface where I can select which IoT device
      then I enter it's control panel (but this is down the line)
      First things first: Create control panel for LED-Arduino-->