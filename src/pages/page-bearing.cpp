#include "pages/page-bearing.hpp"

HTTPServer myServer = HTTPServer();

uint32_t timeBearing = millis();

uint8_t requests = 0;

void handleRoot(HTTPRequest * req, HTTPResponse * res);
void handleEcho(HTTPRequest * req, HTTPResponse * res);

void pageBearing(bool initialLoading)
{
  if (initialLoading)
  {
    // deactivateWifi();
    initDrawBearing();
    initDrawTemperature();
    initMPU();
  }
  if (millis() - timeBearing > 300)
  {
    refreshDrawBearing(getBearing());
    timeBearing = millis();
    float temperature = getTemperature();
    refreshDrawTemperature(temperature);
  }
}

void actionBearing() {
  /*
  msgInfo("Move wristband\nin 8 figure");
  int result = calibrateBearing();
  if (result < 0)
  {
    msgError("Calibration failed");
  }
  else
  {
    msgSuccess("Calibration Successful");
  }
  sleep(5);
  */
  activateWifi();
  ResourceNode * nodeRoot = new ResourceNode("/", "GET", &handleRoot);
  ResourceNode * nodeEchoPost = new ResourceNode("/", "POST", &handleEcho);
  myServer.registerNode(nodeRoot);
  myServer.registerNode(nodeEchoPost);
  myServer.start();
  uint32_t previousTime = millis();
  while (millis() - previousTime <= 10000) {
    myServer.loop();
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(TL_DATUM);
    tft.drawString(String(requests), tft.width() / 2, 100);
  }
}

void handleRoot(HTTPRequest * req, HTTPResponse * res) {
	// We want to deliver an HTML page, so we set the content type
	res->setHeader("Content-Type", "text/html");
	// The response implements the Print interface, so you can use it just like
	// you would write to Serial etc.
	res->println("<!DOCTYPE html>");
	res->println("<html>");
	res->println("<head><title>Hello World!</title></head>");
	res->println("<body>");
	res->println("<h1>Hello World!</h1>");
	res->print("<p>... from your ESP32!</p>");
	res->println("</body>");
	res->println("</html>");
}

void handleEcho(HTTPRequest * req, HTTPResponse * res) {
  requests++;
  // The echo callback will return the request body as response body.
  /*
  zf_init(1);
	zf_bootstrap();
	zf_eval(": . 1 sys ;");
  */

  // We use text/plain for the response
  res->setHeader("Content-Type","text/plain");

  // Stream the incoming request body to the response body
  // Theoretically, this should work for every request size.
  byte buffer[256];
  // HTTPReqeust::requestComplete can be used to check whether the
  // body has been parsed completely.
  while(!(req->requestComplete())) {
    // HTTPRequest::readBytes provides access to the request body.
    // It requires a buffer, the max buffer length and it will return
    // the amount of bytes that have been written to the buffer.
    size_t s = req->readBytes(buffer, 256);

    // The response does not only implement the Print interface to
    // write character data to the response but also the write function
    // to write binary data to the response.
    res->write(buffer, s);
    /*
    zf_result r = zf_eval((char*)buffer);
    if (r != ZF_OK) { res->printf("\n[%d] ERROR", r); }
    */
  }
}

/*
zf_input_state zf_host_sys(zf_syscall_id id, const char *input) {
	char buf[16];

	switch((int)id) {

		case ZF_SYSCALL_EMIT:
			putchar((char)zf_pop());
			fflush(stdout);
			break;

		case ZF_SYSCALL_PRINT:
			itoa(zf_pop(), buf, 10);
			puts(buf);
			break;
	}

	return (zf_input_state)0;
}


zf_cell zf_host_parse_num(const char *buf) {
	char *end;
        zf_cell v = strtol(buf, &end, 0);
	if(*end != '\0') {
                zf_abort(ZF_ABORT_NOT_A_WORD);
        }
        return v;
}
*/
