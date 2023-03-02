/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahabachi <ahabachi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/00/04 10:29:32 by ahabachi          #+#    #+#             */
/*   Updated: 2023/00/04 13:10:39 by ahabachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <pocketsphinx.h>

int main(int argc, char* argv[]) {
  // Initialize the library
  err_init();
  ps_decoder_t* ps = NULL;
  cmd_ln_t* config = NULL;
  ad_rec_t* ad = NULL;

  // Load the configuration
  config = cmd_ln_init(NULL, ps_args(), TRUE,
                       "-hmm", MODELDIR "/hmm/en_US/hub4wsj_sc_8k",
                       "-lm", MODELDIR "/lm/en/turtle.DMP",
                       "-dict", MODELDIR "/lm/en/turtle.dic",
                       NULL);
  if (config == NULL) {
    fprintf(stderr, "Failed to create config object\n");
    return 1;
  }

  // Create the decoder
  ps = ps_init(config);
  if (ps == NULL) {
    fprintf(stderr, "Failed to create decoder object\n");
    return 1;
  }

  // Open the audio file
  char* audio_file = argv[1];
  ad = ad_open_file(audio_file, ad_mode_read);
  if (ad == NULL) {
    fprintf(stderr, "Failed to open audio file '%s'\n", audio_file);
    return 1;
  }

  // Process the audio data
  int16 buf[512];
  int rv;
  while ((rv = ad_read(ad, buf, 512)) >= 0) {
    ps_process_raw(ps, buf, rv, FALSE, FALSE);
  }

  // Extract the speech segments
  int16 const* speech = ps_get_speech(ps);
  int speech_len = ps_get_speech_len(ps);

  // Perform speech recognition
  char const* hyp = ps_get_hyp(ps, NULL);
  printf("Recognized text: %s\n", hyp);

  // Clean up
  ad_close(ad);
  ps_free(ps);
  cmd_ln_free_r(config);

  return 0;
}
